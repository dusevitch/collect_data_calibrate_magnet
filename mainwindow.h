#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once
#include <QMainWindow>
#include <QKeySequence>
//#include "magsensor.h"
#include <eigen3/Eigen/Dense>
#include <QTimer>
#include "PolarisSpectra.h"
#include <polaristransformmatrix.h>
#include <electromagnet_calibration.h>
#include <EigenToYAML.h>
#include <scalorPotential.h>
#include <Timer/Timer.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // ----------------FUNCTIONS----------------

    void getMagPointData();
//    void calibrateMagnet();

    polarisTransformMatrix* getPoseData(int polaris_num);
    void getTrackerPosition(PolarisSpectra *polaris);

    void updateStaticMarkers();

    bool updateCurrPos();

    void startPolaris();
    void stopPolaris();

    void getCoilVals();
    void getCalibData(ElectromagnetCalibration &calibration, std::vector< MagneticMeasurement> &dataList);


    // ----------------VARIABLES----------------
    polarisTransformMatrix *tracker_base_pose;
    polarisTransformMatrix *tracker_wand_pose;

    PolarisSpectra* polaris;

    double tracker_wand_x_val = -1000;
    double tracker_wand_y_val = -1000;
    double tracker_wand_z_val = -1000;

    double tracker_base_x_val = -1000;
    double tracker_base_y_val = -1000;
    double tracker_base_z_val = -1000;

    double coil0 = 0;
    double coil1 = 0;
    double coil2 = 0;

//    ElectromagnetCalibration current_calibration;
//    std::vector<MagneticMeasurement> current_data_list;

//    std::vector<MagneticMeasurement> dataList1;
//    std::vector<MagneticMeasurement> dataList20;
//    std::vector<MagneticMeasurement> dataList40;

//    ElectromagnetCalibration omnimagnet_cal_1amp = ElectromagnetCalibration("initial_guess.yaml");
//    ElectromagnetCalibration omnimagnet_cal_20amp = ElectromagnetCalibration("initial_guess.yaml");
//    ElectromagnetCalibration omnimagnet_cal_40amp = ElectromagnetCalibration("initial_guess.yaml");

    Eigen::Matrix3d sensor_to_RHS;

private slots:
    void on_activate_mag_toggled(bool checked);

    void on_start_sensor_toggled(bool checked);

    void on_start_polaris_toggled(bool checked);

    void on_collect_data_toggled(bool checked);



private:
    Ui::MainWindow *ui;

    Timer polarisTimer;


};

#endif // MAINWINDOW_H
