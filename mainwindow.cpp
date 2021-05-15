#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QFileDialog>
#include <QString>
#include <QTextCodec>


polarisTransformMatrix* buildStructfromTransMatrix(Eigen::Matrix4d &trans_mat);
Eigen::Matrix4d inverseTransformationMatrix(const Eigen::Matrix4d &trans_mat);
void printVector3d(Eigen::Vector3d vec, std::string name);
void printMatrix3d(Eigen::Matrix3d matrix, std::string name);
Eigen::Vector3d averageMatrixValues(const Eigen::Matrix3d & mat);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    innerCoilGain = 12.498;
    middleCoilGain = 13.53;
    outerCoilGain = 9.579;

    ampGains.append(innerCoilGain);
    ampGains.append(middleCoilGain);
    ampGains.append(outerCoilGain);

    shutDownTemp = 60; //Set the shutdown temp to 60 degrees
    shutDownDiff = 5; //If the sensors disagree by 5 degrees
    //Set Up insertion device
    //insertionDevice = new ActsAsynch();

    sensorController = new SensorControl(shutDownTemp);
    sensorController->start_Threading(30);
    // Matrix from the sensor frame to a right handed coordinate frame
    // This frame is the frame aligned with the table coordinate frame
    // when the sensor handle extends towards the drawer, and the sensor
    // is on top.

    innerCoilGain = 12.498;
    middleCoilGain = 13.53;
    outerCoilGain = 9.579;

    ampGains.append(innerCoilGain);
    ampGains.append(middleCoilGain);
    ampGains.append(outerCoilGain);
    sensorController->setAmpGains(ampGains);

    sensor_to_RHS << 1, 2, 3,
                     4, 5, 6,
                     7, 8, 9;

    // connect the GUI_Update slot to get Magnet Temperatures
    counter = new QTimer;
    connect(counter, SIGNAL(timeout()),this,SLOT(GUI_Update()));
    counter->start(25);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_activate_mag_toggled(bool checked)
{
    if (checked){
        // Activate the specific coil value set

        // Check if values in the coils (check if values greater than 40 amps and set error message
        getCoilVals();

            // read the coil values and set an error message
        if (coil0>40.0 || coil0<0.0  || coil1>40.0 || coil1<0.0  || coil2>40.0 || coil2<0.0 ){
            // Error message
            QMessageBox msgBox;
            msgBox.setText("Invalid Values for coils. Keep between 0 amps and 40 amps");
            msgBox.exec();

            sensorController->sendVal(0,0.00);
            sensorController->sendVal(1,0.00);
            sensorController->sendVal(2,0.00);

            ui->activate_mag->setStyleSheet("background-color: none");
            ui->activate_mag->setText("ACTIVATE MAG");
        }else{
            // Turn on the coils (using the same code that is used in Cam's gui
            sensorController->sendAmps(0,coil0);
            sensorController->sendAmps(1,coil1);
            sensorController->sendAmps(2,coil2);
            ui->activate_mag->setStyleSheet("background-color: red");
            ui->activate_mag->setText("DEACTIVATE MAG");
        }
    }else{
        sensorController->sendVal(0,0.00);
        sensorController->sendVal(1,0.00);
        sensorController->sendVal(2,0.00);
        ui->activate_mag->setStyleSheet("background-color: none");
        ui->activate_mag->setText("ACTIVATE MAG");
    }
}


void MainWindow::readMagData(){
    QVector<double> temp(3);

        sensorController->magSensor->MagSenseReadSensor(temp);

            //Change LCD display in the Magnetic Calibration tab (using the sensor)
            ui->mag_sense_x->display(temp[0]);
            ui->mag_sense_y->display(temp[1]);
            ui->mag_sense_z->display(temp[2]);

            // Globally accessable variables to store data in
            mag_xField = temp[0];
            mag_yField = temp[1];
            mag_zField = temp[2];
}

void MainWindow::getCoilVals(){
    QLineEdit *coil_0_val = MainWindow::findChild<QLineEdit *>("coil_0_current");
    QLineEdit *coil_1_val = MainWindow::findChild<QLineEdit *>("coil_1_current");
    QLineEdit *coil_2_val = MainWindow::findChild<QLineEdit *>("coil_2_current");

    QString coil0String =coil_0_val->text();
    coil0=coil0String.toDouble();

    QString coil1String =coil_1_val->text();
    coil1=coil1String.toDouble();

    QString coil2String=coil_2_val->text();
    coil2=coil2String.toDouble();
}

void MainWindow::on_start_sensor_toggled(bool checked)
{
    // activate the sensor (using the same code as in Cam's gui
}

void MainWindow::on_start_polaris_toggled(bool checked)
{
    if (checked){
        startPolaris();
    }else{
        stopPolaris();
    }
}


void MainWindow::on_collect_data_toggled(bool checked)
{
    if (checked){
        QLineEdit *time_to_collect = MainWindow::findChild<QLineEdit *>("collect_time_s");
        QString collect_time_string =time_to_collect->text();
        int collect_time=collect_time_string.toInt()*1000;

        timer = new QTimer();

        qDebug()<< collect_time<<endl;
        time = collect_time;

        // Start the timer for collected seconds and collect data from the wand position at that location (at 30 Hz)

        connect(timer, SIGNAL(timeout()), this, SLOT(getCalibData()));
        timer->start(sampling_rate_ms);


    }else{
        timer->stop();

    }

}

void MainWindow::getCalibData(){

    time = time-sampling_rate_ms;
    ui->timer_cur->display(time/1000.0);
    if (time <=0){
        timer->stop();
    }

    // 1 - Get Position of Tracker (in m not mm) with respect to the base magnet pose
    Vector3d sensor_pos = Vector3d(tracker_wand_x_val,tracker_wand_y_val,tracker_wand_z_val); // This already takes into account the transformation into the base frame
    Vector3d cur_wand_pos = Vector3d(sensor_pos[0]/1000,sensor_pos[1]/1000,sensor_pos[2]/1000);

    // 2 - Proper field: -------------------------------------
    // Make sure this is transformed by the coordinate frame of the sensor,
    // the position of the sensor with respect to the tracker

    // initial field value:  Store Field, in Tesla (Divide mT by 1000)
    Vector3d cur_Field_init = Vector3d(mag_xField/1000,mag_yField/1000,mag_zField/1000);

        // a) get the coord frame of the magnet sensor in wand frame (get into the right handed coordinate frame)
        Matrix3d w_R_s;
        w_R_s << 0, 0, -1,
                 0, -1, 0,
                 1, 0, 0;

        // b) m_R_p * p_R_w * w_R_s * s_F = m=magnet frame, p=polaris frame, w=wand frame, s=sensor frame,F=field vector
        Vector3d cur_Field =  tracker_base_pose->inv_rot_mat * tracker_wand_pose->rot_mat * w_R_s * cur_Field_init;

    // 3 - Get Current Applied in Amps
    getCoilVals();
    Eigen::Vector3d current_vec;
    current_vec << coil0, coil1, coil2;

    // Optional: Print Output
    qDebug() << "---------time = " << time << endl;
//    printVector3d(cur_Field, "Current Field");
//    printVector3d(tracker_wand_pose->pos, "Current Position");
//    printMatrix3d(tracker_wand_pose->rot_mat, "Current Rot");

    qDebug() << cur_Field[0]<<","<<cur_Field[1]<<","<<cur_Field[2]<<","<<cur_wand_pos[0]<<","<<cur_wand_pos[1]<<","<<cur_wand_pos[2]<<","<<current_vec[0]<<","<<current_vec[1]<<","<<current_vec[2];

    // Apply data to vectors to save as file
    cur_Field_vec.push_back(cur_Field);
    tracker_pos.push_back(cur_wand_pos);
    cur_current_vec.push_back(current_vec);

}

void MainWindow::on_save_coil_vals_clicked()
{
    QLineEdit *calib_filename = MainWindow::findChild<QLineEdit *>("filename_calib");
    QString filename = calib_filename->text();
    QString path{"output_calib_files/"};

    QFile file(path+filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    for(int i =0;i<cur_Field_vec.size();i++){
        out << cur_Field_vec[i][0] << "," << cur_Field_vec[i][1] << "," << cur_Field_vec[i][2] << "," <<
        tracker_pos[i][0] << "," << tracker_pos[i][1] << "," << tracker_pos[i][2] << "," <<
        cur_current_vec[i][0] << "," << cur_current_vec[i][1] << "," << cur_current_vec[i][2] << "\n";

    }

    file.close();

    // Clear the values in the vectors
    cur_Field_vec.clear();
    tracker_pos.clear();
    cur_current_vec.clear();

    qDebug() << "Calibration written to: " << (path+filename);

}


void CalibrateData(){
    //for all the files here

    //load the data

    // put each of the data in magnetic measurements

    // run full calibration

    // save the calibration

//// Save the data in a data structure, and eliminate any data outside the workspace bounds
//    MagneticMeasurement cur_measurement;
//    if (calibration.pointInWorkspace(tracker_wand_pose->pos)){
//        cur_measurement =  MagneticMeasurement (cur_Field, tracker_wand_pose->pos, current_vec);
//    }

//  // Put this data into the format for the electromagnet calibration
//    dataList.push_back(cur_measurement);
}



void MainWindow::startPolaris()
{
    //--------------------------------------------------------------------------
    // Polaris Spectra Device Initialize
    //--------------------------------------------------------------------------
    // create a haptic device handler
    polaris = new PolarisSpectra();
    qDebug() <<"Initializing Polaris system";

    // initialize the polaris system
    polaris->Initialize();

    qDebug() <<"Initialized... now activating ports";

    // activate all the ports
    if (polaris->nActivateAllPorts()){
        qDebug() << "Tools have been activated!" << endl;
    }

    // Start the Polaris system tracking
    if (polaris->nStartTracking() == REPLY_OKAY){
        qDebug() << "Tracking has started!" << endl;}
    else{qDebug() << "Device could not start tracking!" << endl;}


    //--------------------------------------------------------------------------
    // END OF Polaris Spectra Device Initialize
    //--------------------------------------------------------------------------

    // Update the position of the initial global frame
    updateStaticMarkers();


    ui->start_polaris->setText("Stop Polaris");

    if(polarisTimer.startTicking(*this, &MainWindow::updateCurrPos, sampling_rate_ms)){
        qDebug() << "Polaris Update Thread Started (~30Hz)" << endl;
    }else{
        qDebug() << "Polaris Update Thread Started (~30Hz)" << endl;
    }
}

void MainWindow::stopPolaris(){
    if(polarisTimer.stopTicking()){
        qDebug() << "Polaris Update Thread STOPPED" << endl;
    }else{
        qDebug() << "Polaris Update Thread ERROR STOPPING (IT CONTINUETH)(~30Hz)" << endl;
    }

    if (polaris->nStopTracking() == REPLY_OKAY){
        qDebug() << "Tracking has stopped!" << endl;}
    else{qDebug() << "Device could not stop tracking!" << endl;}
}

bool MainWindow::updateCurrPos(){
    // Polaris Tracking
    QLCDNumber *tracker_wand_x = MainWindow::findChild<QLCDNumber *>("tracker_wand_x");
    QLCDNumber *tracker_wand_y = MainWindow::findChild<QLCDNumber *>("tracker_wand_y");
    QLCDNumber *tracker_wand_z = MainWindow::findChild<QLCDNumber *>("tracker_wand_z");

    getTrackerPosition(polaris);

    tracker_wand_x->display(QString::number(tracker_wand_x_val));
    tracker_wand_y->display(QString::number(tracker_wand_y_val));
    tracker_wand_z->display(QString::number(tracker_wand_z_val));

    return true;
}

void MainWindow::on_updateBase_clicked()
{
    updateStaticMarkers();
}

void MainWindow::on_start_sensor_clicked(bool checked)
{
    QTimer *mag_sense_timer = new QTimer(this);

    if (checked){
        ui->start_sensor->setText("Stop Mag Sensor");
        sensorController->magSensor->MagSenseBegin();
        connect(mag_sense_timer, SIGNAL(timeout()),this,SLOT(readMagData()));
        mag_sense_timer->start(sampling_rate_ms);
        ui->start_sensor->setStyleSheet("background-color: orange");
    }else{
        disconnect(counter, SIGNAL(timeout()),this,SLOT(readMagData()));
        ui->start_sensor->setText("Start Mag Sensor");
        sensorController->magSensor->MagSenseStop();
        mag_sense_timer->stop();
        ui->start_sensor->setStyleSheet("background-color: none");
    }
}

void MainWindow::getTrackerPosition(PolarisSpectra *polaris){
    polaris->nGetTXTransforms(0);
    tracker_wand_pose = getPoseData(1); // This returns the wand position in the tracker (polaris) frame

    // Position of the wand in the base frame
    Vector3d mag_wand_pos = tracker_base_pose->inv_rot_mat * (tracker_wand_pose->pos-tracker_base_pose->pos);

    tracker_wand_x_val = mag_wand_pos[0];
    tracker_wand_y_val = mag_wand_pos[1];
    tracker_wand_z_val = mag_wand_pos[2];
}


void MainWindow::updateStaticMarkers(){
    polaris->nGetTXTransforms(0);
    tracker_base_pose = getPoseData(2);


    // Position of the base magnet in the base frame
    Vector3d mag_base_pos = tracker_base_pose->inv_rot_mat * (tracker_base_pose->pos-tracker_base_pose->pos);

    tracker_base_x_val = mag_base_pos[0];
    tracker_base_y_val = mag_base_pos[1];
    tracker_base_z_val = mag_base_pos[2];


    QLCDNumber *tracker_base_x = MainWindow::findChild<QLCDNumber *>("tracker_base_x");
    QLCDNumber *tracker_base_y = MainWindow::findChild<QLCDNumber *>("tracker_base_y");
    QLCDNumber *tracker_base_z = MainWindow::findChild<QLCDNumber *>("tracker_base_z");

    tracker_base_x->display(QString::number(tracker_base_x_val));
    tracker_base_y->display(QString::number(tracker_base_y_val));
    tracker_base_z->display(QString::number(tracker_base_z_val));

}

polarisTransformMatrix* MainWindow::getPoseData(int polaris_num){
    polarisTransformMatrix* pose_struct = new polarisTransformMatrix(); // output struct

    if (polaris->m_dtHandleInformation[polaris_num].Xfrms.ulFlags == TRANSFORM_VALID ) {
        // Get the position and rotation values from the polaris and store
        Position3d pos_t = polaris->m_dtHandleInformation[polaris_num].Xfrms.translation;
        QuatRotation quatRot = polaris->m_dtHandleInformation[polaris_num].Xfrms.rotation;
        RotationMatrix rot;
        CvtQuatToRotationMatrix(&quatRot, rot);
        Eigen::Matrix4d curr_trans;
        curr_trans << rot[0][0], rot[0][1], rot[0][2], pos_t.x,
                      rot[1][0], rot[1][1], rot[1][2], pos_t.y,
                      rot[2][0], rot[2][1], rot[2][2], pos_t.z,
                      0.0      , 0.0      , 0.0      , 1.0;
        pose_struct = buildStructfromTransMatrix(curr_trans);
    }else{
        //qDebug() << "TRACKER NOT IN RANGE";
    }
    return pose_struct;
    delete pose_struct;
}

Eigen::Matrix4d inverseTransformationMatrix(const Eigen::Matrix4d &trans_mat){
    // Remove the rotation and position matrices
    Eigen::Matrix3d rot = trans_mat.topLeftCorner(3,3);
    Eigen::Vector4d pos = trans_mat.col(3);

    // transpose of the rotation
    Eigen::Matrix3d rot_t = rot.transpose();

    // Generate inverse transformation matrix and return
    Eigen::Vector3d inv_pos = -rot_t*pos.head(3);
    Eigen::Matrix4d inv_trans_mat;
    inv_trans_mat << rot_t(0,0), rot_t(0,1), rot_t(0,2), inv_pos(0),
                     rot_t(1,0), rot_t(1,1), rot_t(1,2), inv_pos(1),
                     rot_t(2,0), rot_t(2,1), rot_t(2,2), inv_pos(2),
                     0,           0,           0,           1;

    return inv_trans_mat;
}

polarisTransformMatrix* buildStructfromTransMatrix(Eigen::Matrix4d &trans_mat){
    polarisTransformMatrix* pose_struct = new polarisTransformMatrix();

    // Rotation
    pose_struct->rot_mat = trans_mat.topLeftCorner(3,3);

    // Inverse Rotation
    pose_struct->inv_rot_mat = (pose_struct->rot_mat).transpose();

    //Position
    Eigen::Vector4d pos_vec = trans_mat.col(3);
    pose_struct->pos = Eigen::Vector3d(pos_vec(0), pos_vec(1), pos_vec(2));

    // Transformation matrix and Inverse Transformation matrix
    pose_struct->trans_mat = trans_mat;
    pose_struct->inv_trans_mat = inverseTransformationMatrix(trans_mat);

    // Return and delete
    return pose_struct;
    delete pose_struct;
}

void printMatrix3d(Eigen::Matrix3d matrix, std::string name)
{
    std::cout << "-------MATRIX: "<< name << " :---- " << std::endl;
    std::cout <<matrix(0,0)<<","<<matrix(0,1) <<","<<matrix(0,2) <<","<<std::endl;
    std::cout <<matrix(1,0)<<","<<matrix(1,1) <<","<<matrix(1,2) <<","<<std::endl;
    std::cout <<matrix(2,0)<<","<<matrix(2,1) <<","<<matrix(2,2) <<std::endl;
    std::cout << "----------------" << std::endl;
}

void printVector3d(Eigen::Vector3d vec, std::string name)
{
    std::cout << "-------VECTOR: "<< name << " :---- " << std::endl;
    std::cout <<vec(0)<<","<<vec(1) <<","<<vec(2)<<std::endl;
    std::cout << "----------------" << std::endl;
}


void MainWindow::on_printMagData_clicked()
{
    printVector3d(Vector3d(mag_xField,mag_yField,mag_zField ), "Magnetic Field");
}


void MainWindow::GUI_Update()
{

    double tempTime;
    double tempXAmps;
    double tempYAmps;
    double tempZAmps;
    double tempTempInner1;
    double tempTempInner2;
    double tempTempMiddle1;
    double tempTempMiddle2;
    double tempTempOuter1;
    double tempTempOuter2;


    //Get all the sensor values from the sensorController Structure
    if(sensorController->read_lock("GUI UPDATE")){

        tempXAmps = sensorController->dataStructure.xAmps->constLast();
        tempYAmps = sensorController->dataStructure.yAmps->constLast();
        tempZAmps = sensorController->dataStructure.zAmps->constLast();

        tempTime = sensorController->dataStructure.xTime->constLast();

        tempTempInner1 = sensorController->dataStructure.inTemp1->constLast();
        tempTempInner2 = sensorController->dataStructure.inTemp2->constLast();
        tempTempMiddle1 = sensorController->dataStructure.midTemp1->constLast();
        tempTempMiddle2 = sensorController->dataStructure.midTemp2->constLast();
        tempTempOuter1 = sensorController->dataStructure.outTemp1->constLast();
        tempTempOuter2 = sensorController->dataStructure.outTemp2->constLast();

        sensorController->rw_unlock();

    }

    //only display every 10 times around if its too fast its impossible to read the numbers in time
    displayCounter++;

    if(displayCounter > 10){

        ui->InnerTempLCD->display(tempTempInner1);
        ui->middleTempLCD->display(tempTempMiddle1);
        ui->outerTempLCD->display(tempTempOuter1);

        ui->InnerTempLCD_2->display(tempTempInner2);
        ui->middleTempLCD_2->display(tempTempMiddle2);
        ui->outerTempLCD_2->display(tempTempOuter2);
    }

    //Check to see if the temperature is locked
    if(sensorController->isTempLocked()){

        this->on_activate_mag_toggled(false);
    }
}

void MainWindow::on_load_calib_file_clicked()
{
      // QFile Dialog
      QFileDialog dialog(this);
      dialog.setFileMode(QFileDialog::AnyFile);
      dialog.setViewMode(QFileDialog::Detail);

      QStringList fileNames;
      if (dialog.exec())
          fileNames = dialog.selectedFiles();

      for ( const auto& i : fileNames  )
      {
        ui->list_textEdit->append(i);
      }
}

void MainWindow::on_save_calib_clicked()
{
    if(calib_complete){

        QLineEdit *full_calib_filename = MainWindow::findChild<QLineEdit *>("saveCalibration");
        QString filename = full_calib_filename->text();
        QString path{"output_calib_files/"};

        calibration.writeCalibration((path+filename).toStdString());

        qDebug() << "Full Calibration File written to: " << (path+filename);
        calib_complete = false; // Reset calibration

    }else{
        QMessageBox msgBox;
        msgBox.setText("No Calibration Has been completed.");
        msgBox.exec();
    }

}

void MainWindow::loadMagFileData(QString filename){
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
       return;

    MagneticMeasurement cur_measurement;

    QTextStream in(&file);
    while (!in.atEnd()) {

        QString full_line = in.readLine();
        QStringList line = full_line.split(',');

        qDebug() << "data: " << line[0] << "," << line[1]<< "," << line[2]<< "," <<  line[3]<< "," << line[4]<< "," << line[5]<< "," << line[6]<< "," << line[7]<< "," << line[8];

        Vector3d curField = Vector3d(line[0].toDouble(),line[1].toDouble(),line[2].toDouble());
        Vector3d curPos = Vector3d(line[3].toDouble(),line[4].toDouble(),line[5].toDouble());
        Vector3d curCurrent = Vector3d(line[6].toDouble(),line[7].toDouble(),line[8].toDouble());

        // check to see if point is in workspace, and add it if it is
        if (calibration.pointInWorkspace(curPos)){
            cur_measurement =  MagneticMeasurement(curField, curPos, curCurrent);
            dataList.push_back(cur_measurement);
        }else{
            qDebug() << "Point not in workspace not added. Vector: [" << curCurrent[0] << ","<< curCurrent[1] << "," << curCurrent[2];
        }
    }
}

void MainWindow::on_run_calib_clicked()
{
    // Load all files listed in the textEdit box
    QString contents = ui->list_textEdit->toPlainText();
    QStringList file_array = contents.split('\n');
    for (int i=0; i<file_array.size();i++){
        // Create MagneticMeasurment Inputs for each of the files and load all data into dataList
        qDebug() << "----------File Name added: "<< file_array[i] << "-----------";
        loadMagFileData(file_array[i]);


        // Run the calibration and watch for the output

        qDebug() << "dataList size: " << dataList.size();
    }

    // Perform calibration
    calibration.calibrate("Calibration", dataList,true ,true , ElectromagnetCalibration::calibration_constraints::HEADING_THEN_POSITION, -1 ,-1,1e-8);

    calib_complete=true;

}

void MainWindow::on_pushButton_2_clicked()
{
    ui->list_textEdit->clear();
}
