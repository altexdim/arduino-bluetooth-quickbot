#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <Arduino.h>
#include "Types.h"
#include "limits.h"
#include "Encoder.h"
#include "PerformanceCounter.h"
#include "IrSensorsCollection.h"

#define DEBUGGER_MIN_DELAY_BETWEEN_UPDATE_MS 1000

class Debugger {
private:
    unsigned long _lastUpdateTime;
    long _debugMode;

    Encoder *_encoders;
    PerformanceCounter &_performanceCounter;
    IrSensorsCollection &_sensorsCollection;
    Stream &_stream;

public:
    Debugger(
        Encoder *encoders,
        PerformanceCounter &performanceCounter,
        IrSensorsCollection &sensorsCollection,
        Stream &stream
    );
    void update();
    long getDebugMode();
    void setDebugMode(long debugMode);
    void printDebugInfo();
};

#endif
