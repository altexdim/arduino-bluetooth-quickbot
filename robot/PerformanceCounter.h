#ifndef PERFORMANCECOUNTER_H
#define PERFORMANCECOUNTER_H

#include <Arduino.h>
#include "limits.h"

#define PERFORMANCECOUNTER_MIN_DELAY_BETWEEN_UPDATE_MS 1000

class PerformanceCounter {
private:
    unsigned long _lastUpdateTime;
    long _performanceValue;
    long _loopCounter;

public:
    PerformanceCounter();
    void update();
    long getPerformanceValue();
};

#endif
