/**
 * This class is used as collection manager of the array of IR sensors.
 *
 * It is needed to effectively update all of the sensors
 * in desired amount of time.
 */

#ifndef IRSENSORSCOLLECTION_H
#define IRSENSORSCOLLECTION_H

#include "IrSensor.h"
// Used for max ulong value
#include "limits.h"

// Minimum period of time in microseconds to wait between two mesurements for same one sensor in a group
// Not depended on the number of sensors. You can think of this value as expiration time of each sensor reading.
#define IR_SENSORS_COLLECTION_SAME_SENSOR_REFRESH_PERIOD_US 1000

class IrSensorsCollection {
private:
    // Array of sensors
    IrSensor *_sensors;
    // Total number of sensors
    unsigned int _count;
    // Last time previous sensor was updated
    unsigned long _lastUpdateTime;
    // Next sensor index in array
    unsigned int _currentSensorIndex;
    // Delay between updates of the two sensors.
    // Depends on number of sensors and IR_SENSORS_COLLECTION_SAME_SENSOR_REFRESH_PERIOD_US value.
    unsigned long _minDelayBetweenEverySensorUpdateUs;
public:
    /**
     * Constructor
     *
     * @param IrSensor[] - Array of sensors
     * @param int count - Number of sensors
     */
    IrSensorsCollection(IrSensor *sensors, unsigned int count);
    /**
     * Updates value of the next sensor if array.
     */
    void update();
    /**
     * Returns the number of the sensors in collection
     *
     * @return int count - Number of sensors
     */
    unsigned int count();
    /**
     * Returns sensors array
     *
     * @return IrSensors
     */
    IrSensor *getSensors();
};

#endif
