#include "magsensor.h"

MagSensor::MagSensor(SensorControl *sensePass)
{
    senseController = sensePass;

    magSensor = new QSerialPort();
    period = 100;
    newData = false;
    isStarted = false;
//    cal_data = NULL;
    // Set up initial values using a Udev rule you can set this to always be the same
    portName = "/dev/ttyACM0";
    magSensor->setPortName(portName);
    serialBuffer = "";
    xyzReadings = new QVector<double>(3);

    xyzReadings->fill(0.00,3);

}
MagSensor::MagSensor(){
    magSensor = new QSerialPort();
    period = 100;
    newData = false;
    isStarted = false;
//    cal_data = NULL;
    // Set up initial values using a Udev rule you can set this to always be the same
    portName = "/dev/ttyACM0";
    magSensor->setPortName(portName);
    serialBuffer = "";
    xyzReadings = new QVector<double>(3);

    xyzReadings->fill(0.00,3);
}

bool MagSensor::MagSenseIsStarted(){
    return isStarted;
}

bool MagSensor::MagSenseBegin(){
    //magSensor->setPortName(portName);

    if(!magSensor->open(QSerialPort::ReadOnly)){
        qDebug() << "Failed to connect to Magnetic Sensor" << endl;
        return false;
    }else{
        magSensor->setBaudRate(QSerialPort::Baud115200);
        magSensor->setDataBits(QSerialPort::Data8);
        magSensor->setFlowControl(QSerialPort::NoFlowControl);
        magSensor->setParity(QSerialPort::NoParity);
        magSensor->setStopBits(QSerialPort::OneStop);
        qDebug() << "Successfully Connected to Magnetic Sensor" << endl;

        if(readTimer.startTicking(*this, &MagSensor::threadedFunction, 100.0)){
            qDebug() << "Mag Sensor Update Thread Started" << endl;
            isStarted = true;
            return true;
        }else{
            qDebug() << "Mag Sensor Thread did not start" << endl;
            return false;
        }
    }
}


bool MagSensor::threadedFunction(double elapsed_time) { // print out the elapsed time since the function started getting called
    //Split the buffer every time
    static int counter = 0;
    static int amps = 1;
    static int coil = 0;
    static int dump = 0;
    static bool changeFlag = true;
    static bool started = false;

    if(started = false){
        counter = 0;
        amps = 1;
        coil = 0;
        dump = 0;
        changeFlag = true;
        started = true;
    }


    //Read Sensor on Every Round and add it into the buffer


    serialData = magSensor->readAll();
    serialBuffer += QString::fromStdString(serialData.toStdString());

    //Split the buffer up into its parts
    QStringList bufferSplit = serialBuffer.split("\r\n",QString::SkipEmptyParts);

    //If we have at least one complete value lets read it
    if(bufferSplit.length() > 1 && bufferSplit[0] != ""){
        //Now place all the strings into their variables

        QStringList xyzTemp = bufferSplit[0].split(",");

        if(xyzTemp.length() == 3){
            //Used Rotation matricies to align sensor axis to system coordinate frame
            //static int error = pthread_mutex_lock(&magMtx);//Try to grab lock
            //if(error){
            //    cout << "Write lock error in Mag sensor __FUNCTION__: " << error << endl;
            //}else{


            // No transformation
            xyzReadings->clear();
            xyzReadings->append(xyzTemp[0].toDouble());
            xyzReadings->append(xyzTemp[1].toDouble());
            xyzReadings->append(xyzTemp[2].toDouble());

            //This statement added for calibration of omnimag via magnetic sensor

//            if(cal_data != NULL && cal_data->start){
//                //Start the calibration
//                if(changeFlag){
//                    //Make Sure everything is off
//                    senseController->sendAmpsMagSensor(0,0);
//                    senseController->sendAmpsMagSensor(1,0);
//                    senseController->sendAmpsMagSensor(2,0);

//                    //Turn on the coil that we want
//                    senseController->sendAmpsMagSensor(coil,amps);
//                    //cout << "Sending Amps: "<< amps << "to coil: " << coil << endl;
//                    changeFlag = false;
//                }
//                dump++;
//                //Skip the readings to help with accuracy.
//                if(dump > 5){
//                    //Get the data divide by the amps
//                    switch (coil){
//                    case 0:
//                        cal_data->values[0][counter] = -xyzTemp[2].toDouble()/amps;
//                        cal_data->values[1][counter] = -xyzTemp[1].toDouble()/amps;
//                        cal_data->values[2][counter] = xyzTemp[0].toDouble()/amps;
//                        cout << coil << "," << amps << "," << -xyzTemp[2].toDouble() << "," << -xyzTemp[1].toDouble() << "," << xyzTemp[0].toDouble() << endl;
//                        counter++;
//                        changeFlag = true;
//                        break;
//                    case 1:
//                        cal_data->values[3][counter] = -xyzTemp[2].toDouble()/amps;
//                        cal_data->values[4][counter] = -xyzTemp[1].toDouble()/amps;
//                        cal_data->values[5][counter] = xyzTemp[0].toDouble()/amps;
//                        cout << coil << "," << amps << "," << -xyzTemp[2].toDouble() << "," << -xyzTemp[1].toDouble() << "," << xyzTemp[0].toDouble() << endl;
//                        counter++;
//                        changeFlag = true;
//                        break;
//                    case 2:
//                        cal_data->values[6][counter] = -xyzTemp[2].toDouble()/amps;
//                        cal_data->values[7][counter] = -xyzTemp[1].toDouble()/amps;
//                        cal_data->values[8][counter] = xyzTemp[0].toDouble()/amps;
//                        cout << coil << "," << amps << "," << -xyzTemp[2].toDouble() << "," << -xyzTemp[1].toDouble() << "," << xyzTemp[0].toDouble() << endl;
//                        counter++;
//                        changeFlag = true;
//                        break;
//                    }

//                    if(!(counter % 3)){ //After 3 readings increment the amps
//                        amps++;
//                        if(amps > 15){
//                            amps = 1;
//                        }
//                    }

//                    if(counter >= 45){
//                        coil++;
//                        if(coil >2){
//                            cal_data->start = false;
//                            cout << "Calibration Finished" << endl;
//                            senseController->sendAmpsMagSensor(0,0);
//                            senseController->sendAmpsMagSensor(1,0);
//                            senseController->sendAmpsMagSensor(2,0);
//                            started = false;
//                        }
//                        counter=0;
//                    }

//                    dump = 0;
//                }
//            }
            serialBuffer = "";
        }
    }
    return true;
}


//void MagSensor::passReference(calibrationData & passedInfo){
//    cal_data = &passedInfo;
//}

void MagSensor::MagSenseStop(){
    if(readTimer.stopTicking()){
        isStarted = false;
    }
    magSensor->close();

    qDebug() << "Successfully Disconnected to Magnetic Sensor" << endl;
}

void MagSensor::MagSenseReadSensor(QVector<double> & outputReadings){

    outputReadings = *xyzReadings;

}

MagSensor::~MagSensor(){
    if(magSensor->isOpen()){
        this->MagSenseStop();
    }
}

