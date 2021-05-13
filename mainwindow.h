#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once
#include <QMainWindow>
#include <QKeySequence>
#include <eigen3/Eigen/Dense>
#include <QTimer>
#include "PolarisSpectra.h"
#include <polaristransformmatrix.h>
#include <electromagnet-calibration/EigenToYAML.h>
#include <electromagnet-calibration/electromagnet_calibration.h>
#include <electromagnet-calibration/scalorPotential.h>
#include <Timer/Timer.h>
#include "Sensoray/Sensoray.h"
#include "analogbutter.h"
#include "magsensor.h"
#include "sensorcontrol.h"
#include <mutex>

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

    double innerCoilGain;
    double middleCoilGain;
    double outerCoilGain;

    double mag_xField;
    double mag_yField;
    double mag_zField;

    int time; // timer for the collection countdown
    QTimer *timer; // timer for collecting data


    QTimer * counter;

    QVector<double> ampGains;

    double shutDownTemp;
    double shutDownDiff;

    MagSensor * magSensor;

    SensorControl * sensorController;

//    ElectromagnetCalibration current_calibration;
//    std::vector<MagneticMeasurement> current_data_list;

    std::vector<MagneticMeasurement> dataList1;
    std::vector<MagneticMeasurement> dataList20;
    std::vector<MagneticMeasurement> dataList40;

    ElectromagnetCalibration omnimagnet_cal_1amp = ElectromagnetCalibration("initial_guess.yaml");
    ElectromagnetCalibration omnimagnet_cal_20amp = ElectromagnetCalibration("initial_guess.yaml");
    ElectromagnetCalibration omnimagnet_cal_40amp = ElectromagnetCalibration("initial_guess.yaml");

    Eigen::Matrix3d sensor_to_RHS;

    const double sampling_rate_ms{100.0};

private slots:
    void on_activate_mag_toggled(bool checked);

    void on_start_sensor_toggled(bool checked);

    void on_start_polaris_toggled(bool checked);

    void on_collect_data_toggled(bool checked);

    void on_updateBase_clicked();

    void on_start_sensor_clicked(bool checked);

    void readMagData();

    void getCalibData();


    void on_printMagData_clicked();

private:
    Ui::MainWindow *ui;

    Timer polarisTimer;



};

#endif // MAINWINDOW_H
