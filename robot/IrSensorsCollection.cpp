#include "IrSensorsCollection.h"

IrSensorsCollection::IrSensorsCollection(
    IrSensor *sensors,
    unsigned int count
) :
    _sensors(sensors),
    _count(count)
{
    _lastUpdateTime = 0;
    _currentSensorIndex = 0;
    _minDelayBetweenEverySensorUpdateUs = IR_SENSORS_COLLECTION_SAME_SENSOR_REFRESH_PERIOD_US / _count;
}

void IrSensorsCollection::update() {
    unsigned long currentTime = micros();

    unsigned long timeBetweenUpdates;
    if (_lastUpdateTime <= currentTime) {
        timeBetweenUpdates = currentTime - _lastUpdateTime ;
    } else {
        timeBetweenUpdates = ULONG_MAX - _lastUpdateTime  + currentTime;
    }

    if (timeBetweenUpdates > _minDelayBetweenEverySensorUpdateUs) {
        _sensors[_currentSensorIndex].update();
        _currentSensorIndex = (_currentSensorIndex + 1) % _count;
        _lastUpdateTime = currentTime;
    }
}

unsigned int IrSensorsCollection::count() {
    return _count;
}

IrSensor *IrSensorsCollection::getSensors() {
    return _sensors;
}
