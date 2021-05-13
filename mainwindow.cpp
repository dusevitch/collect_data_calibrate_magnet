#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QFileDialog>
#include <QString>


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


    qDebug() << "---------time = " << time << endl;

    Vector3d cur_Field = Vector3d(mag_xField,mag_yField,mag_zField );
    printVector3d(cur_Field, "Current Field");

    // collect rotation and translation of the wand with respect to the magnet
    printVector3d(tracker_wand_pose->pos, "Current Position");
    printMatrix3d(tracker_wand_pose->rot_mat, "Current Rot");

    getCoilVals();

    Eigen::Vector3d current_vec;
    current_vec << coil0, coil1, coil2;

//    // Get sensor data at this point (field data)
//    // Make sure this data is transformed by the coordinate frame of the sensor, the position of the sensor with respect to the tracker, and

    cur_Field_vec.append(cur_Field);
    tracker_pos.append(tracker_wand_pose->pos);
    cur_current_vec.append(current_vec);

}

void MainWindow::on_save_coil_vals_clicked()
{
    QLineEdit *calib_filename = MainWindow::findChild<QLineEdit *>("filename_calib");
    std::string filename = calib_filename->text().toStdString();
    std::string path{"output_calib_files/"};


    for(int i =0;i<)
    qDebug() << "Calibration written to: " << QString::fromStdString(path+filename);
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
//    // THIS SHOULD BE IN THE SENSOR CODE:
//    // Magnetic Sensor
//    QLCDNumber *mag_sense_x = MainWindow::findChild<QLCDNumber *>("mag_sense_x");
//    QLCDNumber *mag_sense_y = MainWindow::findChild<QLCDNumber *>("mag_sense_y");
//    QLCDNumber *mag_sense_z = MainWindow::findChild<QLCDNumber *>("mag_sense_z");


    // Polaris Tracking
    QLCDNumber *tracker_wand_x = MainWindow::findChild<QLCDNumber *>("tracker_wand_x");
    QLCDNumber *tracker_wand_y = MainWindow::findChild<QLCDNumber *>("tracker_wand_y");
    QLCDNumber *tracker_wand_z = MainWindow::findChild<QLCDNumber *>("tracker_wand_z");

    QLCDNumber *tracker_base_x = MainWindow::findChild<QLCDNumber *>("tracker_base_x");
    QLCDNumber *tracker_base_y = MainWindow::findChild<QLCDNumber *>("tracker_base_y");
    QLCDNumber *tracker_base_z = MainWindow::findChild<QLCDNumber *>("tracker_base_z");

    getTrackerPosition(polaris);

    tracker_wand_x->display(QString::number(tracker_wand_x_val));
    tracker_wand_y->display(QString::number(tracker_wand_y_val));
    tracker_wand_z->display(QString::number(tracker_wand_z_val));

    tracker_base_x->display(QString::number(tracker_base_x_val));
    tracker_base_y->display(QString::number(tracker_base_y_val));
    tracker_base_z->display(QString::number(tracker_base_z_val));

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
    tracker_wand_pose = getPoseData(1);

    tracker_wand_x_val = tracker_wand_pose->pos(0);
    tracker_wand_y_val = tracker_wand_pose->pos(1);
    tracker_wand_z_val = tracker_wand_pose->pos(2);
}

void MainWindow::updateStaticMarkers(){
    polaris->nGetTXTransforms(0);
    tracker_base_pose = getPoseData(2);
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
