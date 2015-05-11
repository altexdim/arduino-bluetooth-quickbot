/**
 * Infrared proximity sensor class.
 *
 * Provides high-level functions to use IR proximity sensor.
 * Incapsulates low-level functions like reading from particular port.
 * Calucates average of number of readings to filter analog values.
 * Uses small buffer to store raw readings.
 */

#ifndef IRSENSOR_H
#define IRSENSOR_H

// Used for analogRead() functions
#include <Arduino.h>

// Number of reading to calculate average value.
#define IR_SENSOR_AVG_FILTER_READINGS_COUNT 25
// Input buffer size. Must be greater or equal to IR_SENSOR_AVG_FILTER_READINGS_COUNT.
// In must be greate than IR_SENSOR_AVG_FILTER_READINGS_COUNT by 2 at lease
// 1 is for delete last value, and 2 for delete last diff value when calculating summs and avgs.
#define IR_SENSOR_READ_BUFFER_SIZE (IR_SENSOR_AVG_FILTER_READINGS_COUNT + 2)
// Maximum allowed noise to signal ratio in percent
#define IR_SENSOR_MAX_NOISE 20

class IrSensor {
private:
    // Pin connected to IR sensor's output.
    unsigned int _pin;
    // Next reading index in buffer.
    unsigned int _currentBufferIndex;
    // Input buffer to store analog readings.
    int _valuesRaw[IR_SENSOR_READ_BUFFER_SIZE];
    // Calculated sum value (is needed for calculating avg value).
    long _valueSum;
    // Calculated sum diff (is needed for calculating noise to signal ratio).
    long _diffSum;
    // Calculated avg value (output signal).
    int _value;
    // Calculated noise to signal value.
    int _diffPerc;
public:
    /**
     * Constructor
     *
     * @param int pin - Pin connected to IR sensor
     */
    IrSensor(unsigned int pin);
    /**
     * Initialization function for port.
     */
    void init();
    /**
     * Reads analog value from sensor.
     */
    void update();
    /**
     * Returns last readed value.
     *
     * Uses average value between
     * last number of readings.
     *
     * @return int - Signal output. "0" if signal is unstable.
     */
    int getDistance();
};

#endif
