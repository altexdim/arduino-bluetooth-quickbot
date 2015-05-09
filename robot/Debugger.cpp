#include "Debugger.h"

Debugger::Debugger(
    Encoder *encoders,
    PerformanceCounter &performanceCounter,
    IrSensorsCollection &sensorsCollection,
    Stream &stream
) :
    _encoders(encoders),
    _performanceCounter(performanceCounter),
    _sensorsCollection(sensorsCollection),
    _stream(stream)
{
    _lastUpdateTime = 0;
    _debugMode = 0;
}

void Debugger::update() {
    unsigned long currentTime = millis();

    unsigned long timeBetweenUpdates;
    if (_lastUpdateTime <= currentTime) {
        timeBetweenUpdates = currentTime - _lastUpdateTime;
    } else {
        timeBetweenUpdates = ULONG_MAX - _lastUpdateTime + currentTime;
    }

    if (timeBetweenUpdates > DEBUGGER_MIN_DELAY_BETWEEN_UPDATE_MS) {
        _lastUpdateTime = currentTime;
        printDebugInfo();
    }
}

long Debugger::getDebugMode() {
    return _debugMode;
}

void Debugger::setDebugMode(long debugMode) {
    _debugMode = debugMode;
}

void Debugger::printDebugInfo() {
    if (_debugMode & 1) {
        Serial.print("left=");
        Serial.print(_encoders[WHEEL_LEFT].getCounter());
        Serial.print(" right=");
        Serial.print(_encoders[WHEEL_RIGHT].getCounter());
        Serial.print(" loops=");
        Serial.print(_performanceCounter.getPerformanceValue());
        Serial.print(" l_vel=");
        Serial.print(_encoders[WHEEL_LEFT].getVelocity());
        Serial.print(" r_vel=");
        Serial.print(_encoders[WHEEL_RIGHT].getVelocity());

        IrSensor *sensors = _sensorsCollection.getSensors();
        unsigned int count = _sensorsCollection.count();
        for (int i = 0; i < count; i++) {
            Serial.print("\tS");
            Serial.print(i);
            Serial.print("=");
            Serial.print(sensors[i].getDistance());
        }
        Serial.println();
    }
}
