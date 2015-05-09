/**
 * Class for debugger.
 *
 * Sets debug mode on/off.
 * If debug mode is set, prints debug info every 1 second to the stream.
 * Debug info contains
 *     - encoders values
 *     - encoders velocity
 *     - ir sensors values
 *     - performance counter
 */

#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <Arduino.h>
#include "Types.h"
#include "limits.h"
#include "Encoder.h"
#include "PerformanceCounter.h"
#include "IrSensorsCollection.h"

// How often print debug info, 1000 means one time in a second
#define DEBUGGER_MIN_DELAY_BETWEEN_UPDATE_MS 1000
// Enable bit for normal debug mode
#define DEBUGGER_DEBUG_MODE_NORMAL_BIT 1

class Debugger {
private:
    unsigned long _lastUpdateTime;
    long _debugMode;

    Encoder *_encoders;
    PerformanceCounter &_performanceCounter;
    IrSensorsCollection &_sensorsCollection;
    Stream &_stream;

    /**
     * Prints debug info
     */
    void _printDebugInfo();

public:
    /**
     * Constructor
     *
     * @param Encoder encoders - Encoders
     * @param PerformanceCounter performanceCounter - Performance counter
     * @param IrSensorsCollection sensorsCollection - Sensors collection
     * @param Stream stream - Output stream
     */
    Debugger(
        Encoder *encoders,
        PerformanceCounter &performanceCounter,
        IrSensorsCollection &sensorsCollection,
        Stream &stream
    );
    /**
     * Prints debug information once in a second
     */
    void update();
    /**
     * Gets current debug mode
     *
     * @return long
     */
    long getDebugMode();
    /**
     * Sets debug mode
     *
     * @param long debugMode - New debug mode
     */
    void setDebugMode(long debugMode);
};

#endif
