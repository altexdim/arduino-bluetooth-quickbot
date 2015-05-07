#include "Encoder.h"

Encoder::Encoder(
    unsigned int pin
) :
    _pin(pin)
{
    _state = 1;
    _direction = 1;

    _lastReadTime = 0;
    _currentBufferIndex = 0;
    _counter = 0;
    _velocity = 0;
    _lastVelocityCountTime = 0;
    _lastVelocityCounterValue = 0;
}

void Encoder::init() {
    pinMode(_pin, INPUT);
}

void Encoder::_updateCounter() {
    unsigned long currentTime = micros();

    unsigned long timeBetweenUpdates;
    if (_lastReadTime <= currentTime) {
        timeBetweenUpdates = currentTime - _lastReadTime;
    } else {
        timeBetweenUpdates = ULONG_MAX - _lastReadTime + currentTime;
    }

    if (timeBetweenUpdates > ENCODER_MIN_DELAY_BETWEEN_READ_US) {
        int currentValue = analogRead(_pin);
        _values[_currentBufferIndex] = currentValue;

        currentValue += _values[(ENCODER_READ_BUFFER_SIZE + _currentBufferIndex - 1) % ENCODER_READ_BUFFER_SIZE];
        currentValue += _values[(ENCODER_READ_BUFFER_SIZE + _currentBufferIndex - 2) % ENCODER_READ_BUFFER_SIZE];

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
        _lastReadTime = currentTime;
    }
}

void Encoder::_updateVelocity() {
    unsigned long currentTime = millis();

    unsigned long timeBetweenVelocityUpdates;
    if (_lastVelocityCountTime <= currentTime) {
        timeBetweenVelocityUpdates = currentTime - _lastVelocityCountTime;
    } else {
        timeBetweenVelocityUpdates = ULONG_MAX - _lastVelocityCountTime + currentTime;
    }

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
