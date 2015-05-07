#include "IrSensor.h"

IrSensor::IrSensor(
    unsigned int pin
) :
    _pin(pin)
{
    _currentBufferIndex = 0;
    _value = 0;
}

void IrSensor::init() {
    // I'm not sure we should set this pin to input beacuse we use ADC function analogRead()
    // It's here for consistency if we will try to use digitalRead() function.
    pinMode(_pin, INPUT);
}

void IrSensor::update() {
    int currentValue = analogRead(_pin);
    // Check if value is correct positive number
    if (currentValue >= 0) {
        _valuesRaw[_currentBufferIndex] = currentValue;

        currentValue += _valuesRaw[(IR_SENSOR_READ_BUFFER_SIZE + _currentBufferIndex - 1) % IR_SENSOR_READ_BUFFER_SIZE];
        currentValue += _valuesRaw[(IR_SENSOR_READ_BUFFER_SIZE + _currentBufferIndex - 2) % IR_SENSOR_READ_BUFFER_SIZE];
        _value = currentValue / IR_SENSOR_AVG_FILTER_READINGS_COUNT;

        _currentBufferIndex = (_currentBufferIndex + 1) % IR_SENSOR_READ_BUFFER_SIZE;
    }
}

int IrSensor::getDistance() {
    return _value;
}