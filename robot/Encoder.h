#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include "limits.h"

#define ENCODER_AVG_FILTER_READINGS_COUNT 3
#define ENCODER_MIN_DELAY_BETWEEN_READ_US 300
#define ENCODER_READ_BUFFER_SIZE 5
#define ENCODER_LOW_VALUE_THRESHOLD 100 * ENCODER_AVG_FILTER_READINGS_COUNT
#define ENCODER_HIGH_VALUE_THRESHOLD 1000 * ENCODER_AVG_FILTER_READINGS_COUNT
#define ENCODER_MIN_DELAY_BETWEEN_VELOCITY_COUNT_MS 1000
#define ENCODER_VELOCITY_SCALER_MS 1000

class Encoder {
private:
    unsigned long _lastReadTime;
    unsigned long _lastVelocityCountTime;
    unsigned int _pin;
    unsigned int _currentBufferIndex;
    unsigned int _state;
    long _counter;
    long _velocity;
    long _lastVelocityCounterValue;
    int _values[ENCODER_READ_BUFFER_SIZE];
    int _direction;

    void _updateCounter();
    void _updateVelocity();

public:
    Encoder(unsigned int pin);
    void init();
    void update();
    void reset();
    void setDirection(int direction);
    long getCounter();
    long getVelocity();
};

#endif
