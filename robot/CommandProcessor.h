#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include "Arduino.h"
#include "Chassis.h"
#include "IrSensorsCollection.h"

class CommandProcessor {
private:
    String _inputBuffer;
    String _outputBuffer;
    Stream &_stream;

    Chassis &_chassis;
    IrSensorsCollection &_sensorsCollection;

    int &_debug;
    unsigned long &_perf;

    int _commandCheck();

    int _executeCommand();
    int _commandReset();
    int _commandGetIrVal();
    int _commandGetEnVel();
    int _commandGetEnVal();
    int _commandStop();
    int _commandSetPwm();
    int _commandGetPwm();
    int _commandEnableDebug();
    int _commandDisableDebug();
    int _commandGetPerf();

public:
    CommandProcessor(
        Stream &stream,
        Chassis &chassis,
        IrSensorsCollection &_sensorsCollection,
        int &debug,
        unsigned long &perf
    );
    void readCommand();
};

#endif
