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
#include "sensorcontrol.h"
#include <polaristransformmatrix.h>


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
//    void passReference(calibrationData & passedInfo);

    bool writeMeasurement();

private:
    QSerialPort * magSensor;
    QString portName;
    QVector<double> * xyzReadings;
    QByteArray serialData;
    QString serialBuffer;
    Timer readTimer;

    SensorControl * senseController;

    int period;
    bool isStarted;

    bool threadedFunction(double elapsed_time);
//    calibrationData * cal_data;
};

#endif // MAGSENSOR_H
