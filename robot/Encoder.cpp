#include "Encoder.h"

Encoder::Encoder(
    unsigned int pin
) :
    _pin(pin)
{
    // default input state is logical HIGH level
    _state = 1;
    // default direction is forward
    _direction = 1;

    _lastReadTime = 0;
    _currentBufferIndex = 0;
    _counter = 0;
    _velocity = 0;
    _lastVelocityCountTime = 0;
    _lastVelocityCounterValue = 0;
}

void Encoder::init() {
    // I'm not sure we should set this pin to input beacuse we use ADC function analogRead()
    // It's here for consistency if we will try to use digitalRead() function
    // But if we will use digitalRead function we need to set INPUT_PULLUP mode
    // accorrding to datasheet for sparkfun encoders.
    pinMode(_pin, INPUT);
}

void Encoder::_updateCounter() {
    unsigned long currentTime = micros();

    // First of all we calculate how many microseconds passed after last reading
    unsigned long timeBetweenUpdates;
    if (_lastReadTime <= currentTime) {
        timeBetweenUpdates = currentTime - _lastReadTime;
    } else {
        timeBetweenUpdates = ULONG_MAX - _lastReadTime + currentTime;
    }

    // If last reading is too old then we need to perform next reading
    if (timeBetweenUpdates > ENCODER_MIN_DELAY_BETWEEN_READ_US) {
        int currentValue = analogRead(_pin);
        // Check if value is correct positive number
        if (currentValue >= 0) {
            // Placing new value to current index in buffer
            _values[_currentBufferIndex] = currentValue;

            // We take last ENCODER_AVG_FILTER_READINGS_COUNT values and calculate average
            currentValue += _values[(ENCODER_READ_BUFFER_SIZE + _currentBufferIndex - 1) % ENCODER_READ_BUFFER_SIZE];
            currentValue += _values[(ENCODER_READ_BUFFER_SIZE + _currentBufferIndex - 2) % ENCODER_READ_BUFFER_SIZE];

            // This is implementation of Schmitt trigger with hysteresis
            if (currentValue <= (ENCODER_LOW_VALUE_THRESHOLD  * ENCODER_AVG_FILTER_READINGS_COUNT)) {
                if (_state) {
                    _state = 0;
                }
            } else if (currentValue >= (ENCODER_HIGH_VALUE_THRESHOLD * ENCODER_AVG_FILTER_READINGS_COUNT)) {
                if (!_state) {
                    _state = 1;
                    _counter += _direction;
                }
            }

            _currentBufferIndex = (_currentBufferIndex + 1) % ENCODER_READ_BUFFER_SIZE;
        }
        _lastReadTime = currentTime;
    }
}

void Encoder::_updateVelocity() {
    unsigned long currentTime = millis();

    // How much time passed after last velocity calculation
    unsigned long timeBetweenVelocityUpdates;
    if (_lastVelocityCountTime <= currentTime) {
        timeBetweenVelocityUpdates = currentTime - _lastVelocityCountTime;
    } else {
        timeBetweenVelocityUpdates = ULONG_MAX - _lastVelocityCountTime + currentTime;
    }

    // Update velocity if last value is too old
    if (timeBetweenVelocityUpdates > ENCODER_MIN_DELAY_BETWEEN_VELOCITY_COUNT_MS) {
        _velocity = (_counter - _lastVelocityCounterValue) * ENCODER_VELOCITY_SCALER_MS / timeBetweenVelocityUpdates;
        _lastVelocityCounterValue = _counter;
        _lastVelocityCountTime = currentTime;
    }
}

void Encoder::update() {
    _updateCounter();
    _updateVelocity();
}

void Encoder::reset() {
    _counter = 0;
    _lastVelocityCounterValue = 0;
    _velocity = 0;
}

void Encoder::setDirection(int direction) {
    if (direction < 0) {
        _direction = -1;
    } else {
        _direction = 1;
    }
}

long Encoder::getCounter() {
    return _counter;
}

long Encoder::getVelocity() {
    return _velocity;
}
