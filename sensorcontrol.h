#ifndef SENSORCONTROL_H
#define SENSORCONTROL_H

#define ON 0
#define OFF 1

#include <QObject>
#include <QString>
#include <QSharedDataPointer>
#include "Timer/Timer.h"
#include "QDebug"
#include "Sensoray/Sensoray.h"
#include <mutex>
#include <QWriteLocker>
#include "thermocouple.h"
#include "analogbutter.h"
#include <pthread.h>
#include <errno.h>
#include "magsensor.h"
#include <QDateTime>


/*
 * This class is responsible for all sensing. It reads forces from all sensors, it reads location of the insertion device
 * it reads currents in the coils from the amplifiers it records time and more. Anything that can be sensed it is done here.
 * Its all saved in a huge data structure called Data (pretty good name I know). This data is all saved to a csv
 * in the directory /home/telerobotics/src/OmniControl/DataOutput after a test occurs.
 *
 */



using namespace std;

class MagSensor;

class SensorControl
{
public:
    SensorControl(Sensoray *obj_sense, double maxTemp_);

    SensorControl( double maxTemp_);

    ~SensorControl();
    inline bool isWriting() {return writing;}
    inline bool isThreading() {return sensorClock.isTicking();}

    bool start_Threading(double period_);
    bool start_Threading();
    bool stop_Threading();
    bool toggle_Threading(bool state);
    bool toggle_Threading();
    bool reset_Thread();
    bool start_Writing();
    bool stop_Writing();
    bool toggle_Writing(bool state);
    bool toggle_Writing();
    bool setPeriod(double period_);

    void updateFileName(QString name_);
    void clearData();

    void lockOutput();
    bool unlockOutput();

    bool isTempLocked();

    bool checkTemperature();

    void sendVal(uint channel, double val);
    void sendValNOTEMPLOCK(int channel,double val);

    bool read_lock(QString functionName);
    bool rw_unlock();

    void setAmpGains(QVector<double> & inputGains);

    void sendAmps(int coil, double value);

    void sendAmps(double x, double y,double z);

    void sendAmpsMagSensor(int coil, double value);

    MagSensor * magSensor;

    QVector<double> * coilGains;




    struct Data{

        //Vectors for  time
        QVector<double> * xTime;

        //Vectors to read Amps
        QVector<double> * xAmps;
        QVector<double> * yAmps;
        QVector<double> * zAmps;


        //Vectors to read temperature
        QVector<double> * inTemp1;
        QVector<double> * midTemp1;
        QVector<double> * outTemp1;

        QVector<double> * inTemp2;
        QVector<double> * midTemp2;
        QVector<double> * outTemp2;

        //mutex sensorMutex;
        //pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
        pthread_rwlock_t mtx = PTHREAD_RWLOCK_INITIALIZER;

        Data()
        {

            xTime = new QVector<double>();


            xAmps = new QVector<double>();
            yAmps = new QVector<double>();
            zAmps = new QVector<double>();

            inTemp1 = new QVector<double>();
            midTemp1 = new QVector<double>();
            outTemp1 = new QVector<double>();

            inTemp2 = new QVector<double>();
            midTemp2 = new QVector<double>();
            outTemp2 = new QVector<double>();


            xTime->clear();
            xTime->squeeze();

            inTemp1->clear();
            inTemp1->squeeze();

            midTemp1->clear();
            midTemp1->squeeze();

            outTemp1->clear();
            outTemp1->squeeze();

            inTemp2->clear();
            inTemp2->squeeze();

            midTemp2->clear();
            midTemp2->squeeze();

            outTemp2->clear();
            outTemp2->squeeze();
        }

        ~Data()
        {
            delete xTime;
            delete xAmps;
            delete yAmps;
            delete zAmps;
            delete inTemp1;
            delete midTemp1;
            delete outTemp1;
            delete inTemp2;
            delete midTemp2;
            delete outTemp2;
        }
    };

    Data dataStructure;

private:

    bool threadedFunction(double elapsed_ms);
    void initialize_626_members();

    QString getDateTimeString();

    bool write_lock();

    float scaleADC;
    float analogInputScale;


    int * buffer;
    int * databuf;

    QReadWriteLock * bufferLock;


    double period;
    double timeLapse;
    double lastTime;
    int threadCount;
    int northSouth{1};

    bool writing;
    bool threading;
    bool tempLock;

    QString outputFileName;

    bool DIP_5;

    double maxTemp;
    double shutDownDiff;
    double offTemp;

    QString shutdownReason;

    float totalTime;

    signed int mm_position;

    QString calibrationString = "FT15742.cal";
    QString comediString = "/dev/comedi0";

    Timer sensorClock;

    Sensoray * sense826;

//    FileHandler dataWrite;

    thermoCouple * inner1;
    thermoCouple * inner2;
    thermoCouple * middle1;
    thermoCouple * middle2;
    thermoCouple * outer1;
    thermoCouple * outer2;

    AnalogButter * innerFilter1;
    AnalogButter * innerFilter2;
    AnalogButter * middleFilter1;
    AnalogButter * middleFilter2;
    AnalogButter * outerFilter1;
    AnalogButter * outerFilter2;

    AnalogButter * xAmpsFilter;
    AnalogButter * yAmpsFilter;
    AnalogButter * zAmpsFilter;

};

#endif // SENSORCONTROL_H
