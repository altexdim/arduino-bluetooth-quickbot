/**
 * Class for performance counter.
 *
 * Counts how many loops the program did in one second.
 * Updates every second.
 */

#ifndef PERFORMANCECOUNTER_H
#define PERFORMANCECOUNTER_H

#include <Arduino.h>
#include "limits.h"

// How often to update performance counter. 1000 means every second.
#define PERFORMANCECOUNTER_MIN_DELAY_BETWEEN_UPDATE_MS 1000

class PerformanceCounter {
private:
    // Last time counter was updated
    unsigned long _lastUpdateTime;
    // Last calculated performance value
    long _performanceValue;
    // The number of the program loops happened after last pereformance calculation
    long _loopCounter;

public:
    /**
     * Constructor
     */
    PerformanceCounter();
    /**
     * Update performance counter
     *
     * Calculate performance counter
     * and increments loop counter.
     */
    void update();
    /**
     * Returns last calculated performance counter.
     *
     * Value is in loops per second.
     */
    long getPerformanceValue();
};

#endif
