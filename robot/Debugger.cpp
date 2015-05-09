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

        // Print debug info once in a second
        _printDebugInfo();
    }
}

long Debugger::getDebugMode() {
    return _debugMode;
}

void Debugger::setDebugMode(long debugMode) {
    _debugMode = debugMode;
}

void Debugger::_printDebugInfo() {
    if (_debugMode & DEBUGGER_DEBUG_MODE_NORMAL_BIT) {
        _stream.print("left=");
        _stream.print(_encoders[WHEEL_LEFT].getCounter());
        _stream.print(" right=");
        _stream.print(_encoders[WHEEL_RIGHT].getCounter());
        _stream.print(" loops=");
        _stream.print(_performanceCounter.getPerformanceValue());
        _stream.print(" l_vel=");
        _stream.print(_encoders[WHEEL_LEFT].getVelocity());
        _stream.print(" r_vel=");
        _stream.print(_encoders[WHEEL_RIGHT].getVelocity());

        IrSensor *sensors = _sensorsCollection.getSensors();
        unsigned int count = _sensorsCollection.count();
        for (int i = 0; i < count; i++) {
            _stream.print("\tS");
            _stream.print(i);
            _stream.print("=");
            _stream.print(sensors[i].getDistance());
        }
        _stream.println();
    }
}
