#include "mainwindow.h"
#include "ui_mainwindow.h"


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
}

MainWindow::~MainWindow()
{
    delete ui;


}

void MainWindow::on_activate_mag_toggled(bool checked)
{

}

void MainWindow::on_start_sensor_toggled(bool checked)
{

}

void MainWindow::on_start_polaris_toggled(bool checked)
{
    startPolaris();
}

void MainWindow::on_collect_data_toggled(bool checked)
{

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
}


void MainWindow::updateCurrPos(){
    // THIS SHOULD BE IN THE SENSOR CODE:
    // Magnetic Sensor
    QLCDNumber *mag_sense_x = MainWindow::findChild<QLCDNumber *>("mag_sense_x");
    QLCDNumber *mag_sense_y = MainWindow::findChild<QLCDNumber *>("mag_sense_y");
    QLCDNumber *mag_sense_z = MainWindow::findChild<QLCDNumber *>("mag_sense_z");


    // Polaris Tracking
    QLCDNumber *mag_wand_x = MainWindow::findChild<QLCDNumber *>("mag_wand_x");
    QLCDNumber *mag_wand_y = MainWindow::findChild<QLCDNumber *>("mag_wand_y");
    QLCDNumber *mag_wand_z = MainWindow::findChild<QLCDNumber *>("mag_wand_z");

    QLCDNumber *mag_base_x = MainWindow::findChild<QLCDNumber *>("mag_base_x");
    QLCDNumber *mag_base_y = MainWindow::findChild<QLCDNumber *>("mag_base_y");
    QLCDNumber *mag_base_z = MainWindow::findChild<QLCDNumber *>("mag_base_z");

    getTrackerPosition(polaris);

    mag_wand_x->display(QString::number(mag_wand_x_val));
    mag_wand_y->display(QString::number(mag_wand_x_val));
    mag_wand_z->display(QString::number(mag_wand_x_val));

    mag_base_x->display(QString::number(mag_base_x_val));
    mag_base_y->display(QString::number(mag_base_y_val));
    mag_base_z->display(QString::number(mag_base_z_val));

}


void MainWindow::getTrackerPosition(PolarisSpectra *polaris){
    polaris->nGetTXTransforms(0);
    sensor_wand_pose = getPoseData(1);

    mag_wand_x_val = sensor_wand_pose->pos(0);
    mag_wand_y_val = sensor_wand_pose->pos(1);
    mag_wand_z_val = sensor_wand_pose->pos(2);
}

void MainWindow::updateStaticMarkers(){
    polaris->nGetTXTransforms(0);
    mag_base_pose = getPoseData(2);

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
    pose_struct->pos = Vector3d(pos_vec(0), pos_vec(1), pos_vec(2));

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
