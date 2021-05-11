#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeySequence>
#include "magsensor.h"
#include <eigen3/Eigen/Dense>
#include <QTimer>
#include "PolarisSpectra.h"
#include <polaristransformmatrix.h>
#include <electromagnet_calibration.h>
#include <EigenToYAML.h>
#include <scalorPotential.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void getMagPointData();
//    void calibrateMagnet();

    polarisTransformMatrix* getPoseData(int polaris_num);
    void getTrackerPosition(PolarisSpectra *polaris);

    void updateStaticMarkers();

    void updateCurrPos();

    void startPolaris();

    polarisTransformMatrix *mag_base_pose;
    polarisTransformMatrix *sensor_wand_pose;

    PolarisSpectra* polaris;

    double mag_wand_x_val = -1000;
    double mag_wand_y_val = -1000;
    double mag_wand_z_val = -1000;

    double mag_base_x_val = -1000;
    double mag_base_y_val = -1000;
    double mag_base_z_val = -1000;

private slots:
    void on_activate_mag_toggled(bool checked);

    void on_start_sensor_toggled(bool checked);

    void on_start_polaris_toggled(bool checked);

    void on_collect_data_toggled(bool checked);

private:
    Ui::MainWindow *ui;



};

#endif // MAINWINDOW_H
