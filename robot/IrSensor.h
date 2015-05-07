#ifndef IRSENSOR_H
#define IRSENSOR_H

#include <Arduino.h>
#include "limits.h"

#define IR_SENSOR_READ_BUFFER_SIZE 5
#define IR_SENSOR_AVG_FILTER_READINGS_COUNT 3

class IrSensor {
private:
    unsigned int _pin;
    unsigned int _currentBufferIndex;
    int _valuesRaw[IR_SENSOR_READ_BUFFER_SIZE];
    int _value;
public:
    IrSensor(unsigned int pin);
    void init();
    void update();
    int getDistance();
};

#endif
