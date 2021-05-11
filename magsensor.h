#ifndef MAGSENSOR_H
#define MAGSENSOR_H

#include <QtSerialPort/qserialport.h>
#include <QtSerialPort/qserialportinfo.h>
#include <QString>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Core>
#include <QDebug>
#include <QStringList>
#include <QVector>
#include <Timer/Timer.h>
#include <pthread.h>
//#include "magcalibration.cpp"
//#include "sensorcontrol.h"


//This is a simple class that controls the magnetic sensor, it has some bugs sometimes you need to unplug the sensor and restart things
//to get everything working. All this class does is read from a serial port on a thread. The magnetic sensor is automatically assigned
// to /dev/ttyACM0 due to a udev rule. (https://www.pjrc.com/teensy/49-teensy.rules) talks about adding the udev rule needed to get the Teensy
//to connect to the computer.

using namespace Eigen;
using namespace std;

class SensorControl;

class MagSensor
{
    public:

    MagSensor();
    MagSensor(SensorControl * sensePass);
    ~MagSensor();
    bool newData;
    void MagSenseStop();
    bool MagSenseBegin();
    void MagSenseReadSensor(QVector<double> &outputReadings);
    bool MagSenseIsStarted();

    //bool manualRead(QVector<double> & outputReadings);
    bool getCalibrationData();
    //sensor_pos rotationVector(QString name);
    bool CalibPointMagSenseBegin();
    void CalibPointMagSenseStop();

    bool writeMeasurement();

    int getCurButtonNumber();
    void setCurButtonNumber(int i);


    void printMatrix3d(Eigen::Matrix3d matrix, std::string name);

    int cur_button_number=0;

private:
    QSerialPort * magSensor;
    QString portName;
    QVector<double> * xyzReadings;
    QByteArray serialData;
    QString serialBuffer;
    Timer readTimer;
    double * calibrationArray;
    SensorControl * senseController;



    int period;
    bool isStarted;


    //pthread_mutex_t magMtx;

    bool threadedFunction(double elapsed_time);

};

#endif // MAGSENSOR_H
