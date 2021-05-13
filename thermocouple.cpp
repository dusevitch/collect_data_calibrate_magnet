#include "thermocouple.h"

// int coil -> 1 Inner -> 2 Middle 3-> Outer

thermoCouple::thermoCouple(int thermoNum_)
{
    thermoNum = thermoNum_;
    disconnected = false;
    noPower = false;
}
double thermoCouple::convertData(double input){
    //May need to calibrate each temperature individually dont make this static just yet

        double temp = input*195.3938 - 242.72763;

        if(temp > 1700){
            disconnected = true;
        }else if(temp < -170){
            noPower = true;
        }else{
            disconnected = false;
            noPower = false;
        }

        return temp;
}
bool thermoCouple::isDisconnected(){
    return disconnected;
}
bool thermoCouple::hasNoPower(){
    return noPower;
}
