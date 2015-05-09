#include "PerformanceCounter.h"

PerformanceCounter::PerformanceCounter(
) {
    _lastUpdateTime = 0;
    _performanceValue = 0;
    _loopCounter = 0;
}

void PerformanceCounter::update() {
    _loopCounter++;

    unsigned long currentTime = millis();

    unsigned long timeBetweenUpdates;
    if (_lastUpdateTime <= currentTime) {
        timeBetweenUpdates = currentTime - _lastUpdateTime;
    } else {
        timeBetweenUpdates = ULONG_MAX - _lastUpdateTime + currentTime;
    }

    if (timeBetweenUpdates > PERFORMANCECOUNTER_MIN_DELAY_BETWEEN_UPDATE_MS) {
        _performanceValue = _loopCounter;
        _loopCounter = 0;
        _lastUpdateTime = currentTime;
    }
}

long PerformanceCounter::getPerformanceValue() {
    return _performanceValue;
}
