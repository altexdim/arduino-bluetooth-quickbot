/**
 * Encoders class.
 *
 * This class provide high-level methods to work with encoders.
 * It incapsulates low-level logic like updating encoder counters and velocity, read and filter analog voltage values.
 */

#ifndef ENCODER_H
#define ENCODER_H

// Provides analogRead() function
#include <Arduino.h>
// Provides maximum ulong value
#include "limits.h"

// How many analog voltage readings do we take to calculate average value
// It's needed to filter input values and reduce noise
#define ENCODER_AVG_FILTER_READINGS_COUNT 3
// What is the lowest delay between two readings of encoder analog signal. In microseconds.
#define ENCODER_MIN_DELAY_BETWEEN_READ_US 300
// How many values to keep in buffer. It needs to be greater or equal to ENCODER_AVG_FILTER_READINGS_COUNT
#define ENCODER_READ_BUFFER_SIZE 5
// Lowest voltage limit that tells us that we have logical zero from encoder
#define ENCODER_LOW_VALUE_THRESHOLD 100
// Greatest voltage limit that tells us that we have logical one from encoder
#define ENCODER_HIGH_VALUE_THRESHOLD 1000
// Minimum delay between two calculations of the velocity. In milliseconds.
// For example 500ms means that we update velocity value not faster then twice in 1 second
#define ENCODER_MIN_DELAY_BETWEEN_VELOCITY_COUNT_MS 1000
// Period in milliseconds to extrapolate/interpolate velocity for.
// For example 60000ms means that we calculate velocity in ticks per minute (60000ms=60sec=1min)
#define ENCODER_VELOCITY_SCALER_MS 1000

class Encoder {
private:
    // Last time we've read analog value from encoder
    unsigned long _lastReadTime;
    // Last time we've calculated velocity of the wheel
    unsigned long _lastVelocityCountTime;
    // The PIN number encoder connected to
    unsigned int _pin;
    // Index of the next reading in input buffer.
    // Every time we read value from encoder we put it in the buffer by that index,
    // and then we increment index preparing for the next reading
    unsigned int _currentBufferIndex;
    // Last detected state of the encoder input, can be 1 and 0 for logical levels HIGH and LOW
    unsigned int _state;
    // Ticks counter for the encoder
    long _counter;
    // Velocity of the wheel, speed of incrementing counter value
    long _velocity;
    // Last time we've calculated velocity
    long _lastVelocityCounterValue;
    // Input buffer with last analog readings from encoder
    int _values[ENCODER_READ_BUFFER_SIZE];
    // Direction of the wheel. Can be 1 for forward and -1 for backward velocity.
    int _direction;

    /**
     * Updates counter value
     */
    void _updateCounter();
    /**
     * Updates velocity
     */
    void _updateVelocity();

public:
    /**
     * Constructor
     *
     * @param int pin - Pin number encoder connected to
     */
    Encoder(unsigned int pin);

    /**
     * Initialize encoder, set input state to pin.
     */
    void init();
    /**
     * Updates encoder counter and velocity
     */
    void update();
    /**
     * Reset encoder counter and velocity
     */
    void reset();
    /**
     * Set wheel direction
     *
     * @param int direction - positive or zero for forward, negative for backward
     */
    void setDirection(int direction);
    /**
     * Get counter value
     */
    long getCounter();
    /**
     * Get velocity value
     */
    long getVelocity();
};

#endif
