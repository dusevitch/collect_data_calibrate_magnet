#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QFileDialog>
#include <QString>
#include <electromagnet_calibration.h>
#include <EigenToYAML.h>
#include <scalorPotential.h>


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

    // Matrix from the sensor frame to a right handed coordinate frame
    // This frame is the frame aligned with the table coordinate frame
    // when the sensor handle extends towards the drawer, and the sensor
    // is on top.
    sensor_to_RHS << 1, 2, 3,
                     4, 5, 6,
                     7, 8, 9;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_activate_mag_toggled(bool checked)
{
    // Activate the specific coil value set

    // Check if values in the coils (check if values greater than 40 amps and set error message
    getCoilVals();

        // read the coil values and set an error message
    if (coil0>40 || coil0<0  || coil1>40 || coil1<0  || coil2>40 || coil2<0 ){
        // Error message
        QMessageBox msgBox;
        msgBox.setText("Invalid Values for coils. Keep between 0 amps and 40 amps");
        msgBox.exec();

    }else{
        // Turn on the coils (using the same code that is used in Cam's gui

    }
}

void MainWindow::getCoilVals(){
    QLineEdit *coil_0_val = MainWindow::findChild<QLineEdit *>("coil_0_current");
    QLineEdit *coil_1_val = MainWindow::findChild<QLineEdit *>("coil_1_current");
    QLineEdit *coil_2_val = MainWindow::findChild<QLineEdit *>("coil_2_current");

    QString coil0String =coil_0_val->text();
    coil0=coil0String.toDouble();

    QString coil1String =coil_1_val->text();
    coil1=coil1String.toDouble();

    QString coil2String =coil_2_val->text();
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
    QLineEdit *time_to_collect = MainWindow::findChild<QLineEdit *>("coil_0_current");
    QString collect_time_string =time_to_collect->text();
    int collect_time=collect_time_string.toInt();

    // Start the timer for collected seconds and collect data from the wand position at that location (at 30 Hz)
    for (int i=0;i<(34*collect_time);i=i+34){
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(getCalibData(omnimagnet_cal_1amp, dataList1)));
        timer->start(34);
    }
}

void MainWindow::getCalibData(ElectromagnetCalibration &calibration, std::vector< MagneticMeasurement> &dataList){
    // collect rotation and translation of the wand with respect to the magnet
    printVector3d(tracker_wand_pose->pos, "Current Position");
    printMatrix3d(tracker_wand_pose->rot_mat, "Current Rot");

    getCoilVals();
//    Eigen::VectorXd current_vec = Eigen::VectorXd(coil0, coil1, coil2);

    // Get sensor data at this point (field data)
    // Make sure this data is transformed by the coordinate frame of the sensor, the position of the sensor with respect to the tracker, and


//    // Save the data in a data structure, and eliminate any data outside the workspace bounds
//    if (calibration.pointInWorkspace (tracker_wand_pose)){
//        MagneticMeasurement cur_measurement =  MagneticMeasurement (const Eigen::Vector3d &Field, tracker_wand_pose.pos, current_vec);
//    }
//    // Put this data into the format for the electromagnet calibration

//    dataList.push_back(cur_measurement);
}



//void MainWindow::on_actiontestSave_triggered()
//{
//    saveFile();
//}

//void MainWindow::saveFile ()
//{
//    QString fname = QFileDialog::getSaveFileName(nullptr, "test sav e name", ".", "Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml)" );
//    qDebug() << "name is : " << fname;
//}


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

    if(polarisTimer.startTicking(*this, &MainWindow::updateCurrPos, 34.0)){
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

    printVector3d(tracker_wand_pose->pos, "trackerwand");

    tracker_wand_x->display(QString::number(tracker_wand_x_val));
    tracker_wand_y->display(QString::number(tracker_wand_y_val));
    tracker_wand_z->display(QString::number(tracker_wand_z_val));

    tracker_base_x->display(QString::number(tracker_base_x_val));
    tracker_base_y->display(QString::number(tracker_base_y_val));
    tracker_base_z->display(QString::number(tracker_base_z_val));

    return true;
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

