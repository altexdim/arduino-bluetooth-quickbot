#ifndef IRSENSORSCOLLECTION_H
#define IRSENSORSCOLLECTION_H

#include "IrSensor.h"

#define IR_SENSORS_COLLECTION_SAME_SENSOR_REFRESH_PERIOD_US 1000

class IrSensorsCollection {
private:
    IrSensor *_sensors;
    unsigned int _count;
    unsigned long _lastUpdateTime;
    unsigned int _currentSensorIndex;
    unsigned long _minDelayBetweenEverySensorUpdateUs;
public:
    IrSensorsCollection(IrSensor *sensors, unsigned int count);
    void update();
};

#endif
