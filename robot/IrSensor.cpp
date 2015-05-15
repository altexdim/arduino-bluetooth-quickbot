#include "IrSensor.h"

IrSensor::IrSensor(
    unsigned int pin
) :
    _pin(pin)
{
    _currentBufferIndex = 0;
    _value = 0;
    _valueSum = 0;
    _diffSum = 0;
    _diffPerc = 0;
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
        // new measurement value
        _valuesRaw[_currentBufferIndex] = currentValue;
        // remove oldes value, add new value to summ
        _valueSum += currentValue - _valuesRaw[(IR_SENSOR_READ_BUFFER_SIZE + _currentBufferIndex - IR_SENSOR_AVG_FILTER_READINGS_COUNT) % IR_SENSOR_READ_BUFFER_SIZE];
        // calculate agv value
        _value = (int)(_valueSum / IR_SENSOR_AVG_FILTER_READINGS_COUNT);

        // calculate oldest diff
        int diffLast = abs(
            _valuesRaw[(IR_SENSOR_READ_BUFFER_SIZE + _currentBufferIndex - IR_SENSOR_AVG_FILTER_READINGS_COUNT) % IR_SENSOR_READ_BUFFER_SIZE]
            - _valuesRaw[(IR_SENSOR_READ_BUFFER_SIZE + _currentBufferIndex - IR_SENSOR_AVG_FILTER_READINGS_COUNT - 1) % IR_SENSOR_READ_BUFFER_SIZE]
        );
        // calculate new diff
        int diffNew = abs(currentValue - _valuesRaw[(IR_SENSOR_READ_BUFFER_SIZE + _currentBufferIndex - 1) % IR_SENSOR_READ_BUFFER_SIZE]);
        // remove oldes diff, add new diff to summ
        _diffSum += diffNew - diffLast;
        // calculate diff (noise) to value (signal) ration in percents
        _diffPerc = (int)(_diffSum*100 / _valueSum);

        _currentBufferIndex = (_currentBufferIndex + 1) % IR_SENSOR_READ_BUFFER_SIZE;
    }
}

int IrSensor::getDistance() {
    // If noise is greater than IR_SENSOR_MAX_NOISE return 0
    return _diffPerc > IR_SENSOR_MAX_NOISE? 0: _value;
}
