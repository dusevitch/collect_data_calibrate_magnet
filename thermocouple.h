#include "Sensoray/Sensoray.h"

#ifndef THERMOCOUPLE_H
#define THERMOCOUPLE_H

//This is a simple thermocouple class, it converts the output voltage of the thermocouple sensors to a temperature value.


class thermoCouple
{
public:
    thermoCouple(int thermoNum_);
    double convertData(double input);
    bool isDisconnected();
    bool hasNoPower();
private:
    int thermoNum;
    bool disconnected;
    bool noPower;


};

#endif // THERMOCOUPLE_H
