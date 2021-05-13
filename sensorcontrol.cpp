#include "sensorcontrol.h"


SensorControl::SensorControl(Sensoray *obj_sense ,double maxTemp_)
{
    sense826 = obj_sense;
    period = 0;
    threading = false;
    writing = false;
    lastTime = 0;
    tempLock = false;
    DIP_5 = ON; //This is the 5th DIP switch on the drive if its OFF it reduces current by half and changes the


    magSensor = new MagSensor(this);

    maxTemp = maxTemp_;
    shutDownDiff = 11;
    offTemp = -100.0; //Temperature that tells us that the device isnt plugged in

    //pthread_rwlock_init(&dataStructure.mtx, PTHREAD_RWLOCK_PREFER_WRITER_NP)

    inner1 = new thermoCouple(1);
    inner2 = new thermoCouple(2);

    middle1 = new thermoCouple(3);
    middle2 = new thermoCouple(4);

    outer1 = new thermoCouple(5);
    outer2 = new thermoCouple(6);

    innerFilter1 = new AnalogButter(.1,25,true,true);
    innerFilter2 = new AnalogButter(.1,25,true,true);

    middleFilter1 = new AnalogButter(.1,25,true,true);
    middleFilter2 = new AnalogButter(.1,25,true,true);

    outerFilter1 = new AnalogButter(.1,25,true,true);
    outerFilter2 = new AnalogButter(.1,25,true,true);

    xAmpsFilter = new AnalogButter(.15,10,true,true);
    yAmpsFilter = new AnalogButter(.15,10,true,true);
    zAmpsFilter = new AnalogButter(.15,10,true,true);


//    dataWrite.set_binary(false);
    initialize_626_members();


}
SensorControl::SensorControl( double maxTemp_){

    maxTemp = maxTemp_;
    shutDownDiff = 11;
    DIP_5 = ON; //This is the 5th DIP switch on the drive if its OFF it reduces current by half and changes the
    offTemp = -100.0; //Temperature that tells us that the device isnt plugged in

    magSensor = new MagSensor(this);

    northSouth = 1;


    inner1 = new thermoCouple(1);
    inner2 = new thermoCouple(2);

    middle1 = new thermoCouple(3);
    middle2 = new thermoCouple(4);

    outer1 = new thermoCouple(5);
    outer2 = new thermoCouple(6);

    innerFilter1 = new AnalogButter(.1,100,true,true);
    innerFilter2 = new AnalogButter(.1,100,true,true);

    middleFilter1 = new AnalogButter(.1,100,true,true);
    middleFilter2 = new AnalogButter(.1,100,true,true);

    outerFilter1 = new AnalogButter(.1,100,true,true);
    outerFilter2 = new AnalogButter(.1,100,true,true);

//    xAmpsFilter = new AnalogButter(.15,10,true,true);
//    yAmpsFilter = new AnalogButter(.15,10,true,true);
//    zAmpsFilter = new AnalogButter(.15,10,true,true);

    sense826 = new Sensoray;
    initialize_626_members();

}


bool SensorControl::threadedFunction(double elapsed_ms){

    if(this->write_lock()){

        sense826->readIn();// Updates the buffer

        dataStructure.xAmps->append(sense826->get_analog_value(0)*(15.8/(int(DIP_5)+1.0)));
        dataStructure.yAmps->append(sense826->get_analog_value(1)*(15.8/(int(DIP_5)+1.0)));
        dataStructure.zAmps->append(sense826->get_analog_value(2)*(15.8/(int(DIP_5)+1.0)));

        dataStructure.inTemp1->append(inner1->convertData(innerFilter1->update(sense826->get_analog_value(4))));
        dataStructure.inTemp2->append(inner2->convertData(innerFilter2->update(sense826->get_analog_value(5))));
        dataStructure.midTemp1->append(middle1->convertData(middleFilter1->update(sense826->get_analog_value(6))));
        dataStructure.midTemp2->append(middle2->convertData(middleFilter2->update(sense826->get_analog_value(7))));
        dataStructure.outTemp1->append(outer1->convertData(outerFilter1->update(sense826->get_analog_value(8))));
        dataStructure.outTemp2->append(outer2->convertData(outerFilter2->update(sense826->get_analog_value(9))));

        tempLock = checkTemperature();

        dataStructure.xTime->append(elapsed_ms);

        this->rw_unlock();
    }else{
        qDebug() << "Cant get lock to write sensor data! " << endl;
    }

    threadCount++;
    timeLapse += elapsed_ms-lastTime;

    lastTime = elapsed_ms;

    return true;
}

void SensorControl::lockOutput(){
    tempLock = true;
    this->sendVal(0,0.00);
    this->sendVal(1,0.00);
    this->sendVal(2,0.00);
    this->sendVal(3,0.00);
}
bool SensorControl::isTempLocked(){
    return tempLock;
}

bool SensorControl::unlockOutput(){

    if(this->read_lock("unlockOutput")){
        if(checkTemperature()){
            tempLock = false;

        }else{
            //cout << "Coils still too hot" << endl;
            tempLock = true;
        }
    }else{
        qDebug() << "Couldnt check temp didnt get lock" << endl;
        return true;
    }

    return tempLock;
}
bool SensorControl::checkTemperature(){
    double Tin1  = dataStructure.inTemp1->constLast();
    double Tin2  = dataStructure.inTemp2->constLast();
    double Tmid1 = dataStructure.midTemp1->constLast();
    double Tmid2 = dataStructure.midTemp2->constLast();
    double Tout1 = dataStructure.outTemp1->constLast();
    double Tout2 = dataStructure.outTemp2->constLast();

    static int count = 100;
    //check for sensor disagreement
    if(abs(Tin1 - Tin2) > shutDownDiff || abs(Tmid1 - Tmid2) > shutDownDiff || abs(Tout1 - Tout2) > shutDownDiff){
        count++;
        if(count > 1){

            //cout << "Sensor Disagreement...Overwriting Temp Value" << endl;
            //cout << "Before: "<< "Tin1: " <<  Tin1  <<" Tin2: "  << Tin2
            //     <<" Tmid1: " << Tmid1 <<" Tmid2: " << Tmid2
            //     <<" Tout1: " << Tout1 <<" Tout2: " << Tout2 << endl;

            double temps[] = {Tin1,Tin2,Tmid1,Tmid2,Tout1,Tout2};
            //Find the average of all the temperatures except the largest
            double tempAvg = (Tin1+Tin2+Tmid1+Tmid2+Tout1+Tout2-*std::max_element(temps,temps+6))/5;

            //cout << "MAX: " << *std::max_element(temps,temps+6) << endl;
            //If the difference is more than 12 reset the value as it must be a Sensoray Card Error
            if(abs(Tin1-tempAvg) > 12) Tin1 = tempAvg;
            if(abs(Tin2-tempAvg) > 12) Tin2 = tempAvg;
            if(abs(Tmid1-tempAvg) > 12) Tmid1 = tempAvg;
            if(abs(Tmid2-tempAvg) > 12) Tmid2 = tempAvg;
            if(abs(Tout1-tempAvg) > 12) Tout1 = tempAvg;
            if(abs(Tout2-tempAvg) > 12) Tout2 = tempAvg;

            //cout << "After : "<< "Tin1: " <<  Tin1  <<" Tin2: "  << Tin2
            //     <<" Tmid1: " << Tmid1 <<" Tmid2: " << Tmid2
            //     <<" Tout1: " << Tout1 <<" Tout2: " << Tout2 << endl;


            shutdownReason = "Sensor Disagreement, no shut down, make sure each set is plugged in.";
            count = 0;
        }
        //this->lockOutput(); //Thermocouples can give sparatic values at times, getting rid of this so that it doesnt
        //shut down in the middle of an insertion


        return false; //Lock the temp
    }

    //Check for no power
    if(     Tin1 < offTemp ||  Tin2 < offTemp
            || Tmid1 < offTemp || Tmid2 < offTemp
            || Tout1 < offTemp || Tout2 < offTemp){
        count++;

        if(count > 1){
            //cout << "Shutdown due to Power Failure, Plug in thermocouples" << endl;
            shutdownReason = "Shutdown due to Power Failure, Plug in thermocouples";
            count = 0;
        }
        this->lockOutput();
        return true; //lock the temp
    }

    //Check for high Temp
    if(     ((Tin1 + Tin2)/2) > maxTemp ||
            ((Tmid1+Tmid2)/2) > maxTemp ||
            ((Tout1+Tout2)/2) > maxTemp){
        count++;

        if(count > 1){
            //cout << "Shutdown due to High Temperature" << endl;
            shutdownReason = "Shutdown due to High Temperature";
            count = 0;
        }
        this->lockOutput();
        return true; //lock the temp
    }
    shutdownReason = "";
    return false; //Unlock the temp
}

bool SensorControl::setPeriod(double period_){
    period = period_;
    return true;
}

bool SensorControl::start_Threading(double period_){
    period = period_;
    threadCount = 0;
    lastTime = 0;
    timeLapse = 0;
    if (this->sensorClock.startTicking(*this, &SensorControl::threadedFunction, period))
    {
        qDebug() << "Sensor Thead Started at: " << period << endl;
        return true;
    }
    else
    {
        return false;
    }
}
bool SensorControl::start_Threading(){
    threadCount = 0;
    lastTime = 0;
    timeLapse = 0;
    if (this->sensorClock.startTicking(*this, &SensorControl::threadedFunction, period))
    {
        qDebug() << "Sensor Thread Started at: " << period << endl;
        return true;
    }
    else
    {
        qDebug() << "Sensor Thread did not start!!!" << endl;
        return false;
    }
}
bool SensorControl::reset_Thread(){
    stop_Threading();
    start_Threading();
    return true;
}

bool SensorControl::stop_Threading(){
    if(this->sensorClock.stopTicking()){
        qDebug() << "Thread Stopped with an average period of: "<< timeLapse/(threadCount-1) <<endl;
        timeLapse = 0;
        threadCount = 0;

        return true;
    }else{
        return false;
    }
}
bool SensorControl::toggle_Threading(bool state){
    if(state){
        if(this->start_Threading(period)){
            return true;
        }
        return false;
    }else{
        if(this->stop_Threading()){
            return true;
        }
        return false;
    }
}
bool SensorControl::toggle_Threading(){

    if(!sensorClock.isTicking()){
        this->start_Threading(period);
        return true;
    }else{
        this->stop_Threading();
        return true;
    }
    return false;
}
bool SensorControl::start_Writing(){
    writing = true;
    return true;
}
bool SensorControl::stop_Writing(){
    writing = false;
    return true;
}
bool SensorControl::toggle_Writing(bool state){
    writing = state;
    return true;
}
bool SensorControl::toggle_Writing(){
    writing = !writing;
    return true;
}
bool SensorControl::read_lock(QString functionName){
    static int error = pthread_rwlock_tryrdlock(&dataStructure.mtx);//Try to grab lock

    //Check if it has returned an error
    if(error){
        qDebug() << "Could not get the read lock, Error: "<< error << endl;
        return false;
    }else{
        //If we get the lock check if the data is there
        if(dataStructure.xAmps->isEmpty()){

            qDebug() << "Missed Data: Vectors are Empty, Called by function: " << functionName << endl;

            return false;
        }else{
            //Return true (Got lock and data exists)
            return true;
        }

    }
}
bool SensorControl::write_lock(){
    static int error = pthread_rwlock_trywrlock(&dataStructure.mtx);//Try to grab lock
    //Check if it has returned an error
    if(error){
        qDebug() << "Could not get the write lock, Error: "<< error << endl;
        return false;
    }else{
        //Return true (Got lock)
        return true;
    }
}
bool SensorControl::rw_unlock(){
    static int error = pthread_rwlock_unlock(&dataStructure.mtx);
    //dataStructure.sensorMutex.unlock();
    if(error){
        qDebug() << "Mutex failed to unlock ERROR: " << error << endl;
        return false;
    }
    return true;
}

void SensorControl::initialize_626_members(){

    this->bufferLock = new QReadWriteLock(); // initialize the read write locker
    this->buffer = sense826->returnBufferPointer();
    this->scaleADC = sense826->returnAnalogInputScale();
}
void SensorControl::clearData(){

    this->read_lock("Clear Data");

    dataStructure.xAmps->clear();
    dataStructure.xAmps->squeeze();

    dataStructure.yAmps->clear();
    dataStructure.yAmps->squeeze();

    dataStructure.zAmps->clear();
    dataStructure.zAmps->squeeze();

    dataStructure.xTime->clear();
    dataStructure.xTime->squeeze();

    dataStructure.inTemp1->clear();
    dataStructure.inTemp1->squeeze();

    dataStructure.inTemp2->clear();
    dataStructure.inTemp2->squeeze();

    dataStructure.midTemp1->clear();
    dataStructure.midTemp1->squeeze();

    dataStructure.midTemp2->clear();
    dataStructure.midTemp2->squeeze();

    dataStructure.outTemp1->clear();
    dataStructure.outTemp1->squeeze();

    dataStructure.outTemp2->clear();
    dataStructure.outTemp2->squeeze();

    innerFilter1->resetCounter();
    innerFilter2->resetCounter();

    middleFilter1->resetCounter();
    middleFilter2->resetCounter();

    outerFilter1->resetCounter();
    outerFilter2->resetCounter();

    this->rw_unlock();
}

void SensorControl::sendVal(uint channel, double val){
    if(!this->tempLock){
        sense826->sendVoltage(channel,val);
    }else{
        sense826->sendVoltage(channel,0.00);
    }

}

void SensorControl::sendAmps(int coil, double value){

    this->sendValNOTEMPLOCK(coil,value/coilGains->at(coil));

}
void SensorControl::sendAmpsMagSensor(int coil, double value){

    this->sendValNOTEMPLOCK(coil,value/coilGains->at(coil));

}

void SensorControl::sendAmps(double x, double y, double z){

    this->sendAmps(0,x);
    this->sendAmps(1,y);
    this->sendAmps(2,z);

    cout << "Sending Amps (x,y,z): " << x << ", " << y << ", " << z << endl;

}

//THIS FUNCTION DOES NOT CHECK IF THE TEMPERATURE IS TOO HIGH BEFORE IT SENDS THE VALUE BE VERY CAREFUL WITH THIS
void SensorControl::sendValNOTEMPLOCK(int channel, double val){
    sense826->sendVoltage(channel,val);
}

void SensorControl::updateFileName(QString name_){
    outputFileName = name_;
}

QString SensorControl::getDateTimeString(){
    QDateTime currentTime = QDateTime::currentDateTime();
    return currentTime.toString("dd-MM-yy-hh-mm-ss");
}
void SensorControl::setAmpGains(QVector<double> & inputGains){

    coilGains = &inputGains;

    cout << "coilGains: " << coilGains->at(0)<<" , "<< coilGains->at(1)<<" , "<< coilGains->at(2) << endl;

}

SensorControl::~SensorControl()
{
    if(this->isWriting()){
        this->stop_Writing();
    }
    if(this->isThreading()){
        this->stop_Threading();
    }

    delete sense826;


}
