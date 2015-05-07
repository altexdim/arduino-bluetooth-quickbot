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

// Input buffer size. Must be greater or equal to IR_SENSOR_AVG_FILTER_READINGS_COUNT.
#define IR_SENSOR_READ_BUFFER_SIZE 5
// Number of reading to calculate average value.
#define IR_SENSOR_AVG_FILTER_READINGS_COUNT 3

class IrSensor {
private:
    // Pin connected to IR sensor's output.
    unsigned int _pin;
    // Next reading index in buffer.
    unsigned int _currentBufferIndex;
    // Input buffer to store analog readings.
    int _valuesRaw[IR_SENSOR_READ_BUFFER_SIZE];
    // Calculated average value.
    int _value;
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
     */
    int getDistance();
};

#endif
