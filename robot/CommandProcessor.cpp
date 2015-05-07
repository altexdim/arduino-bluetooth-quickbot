#include "CommandProcessor.h"

CommandProcessor::CommandProcessor(
    Stream &stream,
    Chassis &chassis,
    IrSensorsCollection &_sensorsCollection,
    int &debug,
    unsigned long &perf
) :
    _stream(stream),
    _chassis(chassis),
    _sensorsCollection(_sensorsCollection),
    _debug(debug),
    _perf(perf)
{
}

int CommandProcessor::_commandCheck() {
    _outputBuffer = "Hello from QuickBot";
    return 1;
}

int CommandProcessor::_commandReset() {
    _chassis.resetEncoders();
    _outputBuffer = "Encoder values reset to [0, 0]";
    return 1;
}

int CommandProcessor::_commandGetIrVal() {
    _outputBuffer.concat("[");
    IrSensor *sensors = _sensorsCollection.getSensors();
    unsigned int count = _sensorsCollection.count();
    for (int i = 0; i < count; i++) {
        if (i) {
            _outputBuffer.concat(", ");
        }
        _outputBuffer.concat(sensors[i].getDistance());
    }
    _outputBuffer.concat("]");
    return 1;
}

int CommandProcessor::_commandGetEnVel() {
    Encoder *encoders = _chassis.getEncoders();
    _outputBuffer.concat("[");
    _outputBuffer.concat(encoders[WHEEL_LEFT].getVelocity());
    _outputBuffer.concat(", ");
    _outputBuffer.concat(encoders[WHEEL_RIGHT].getVelocity());
    _outputBuffer.concat("]");
    return 1;
}

int CommandProcessor::_commandGetEnVal() {
    Encoder *encoders = _chassis.getEncoders();
    _outputBuffer.concat("[");
    _outputBuffer.concat(encoders[WHEEL_LEFT].getCounter());
    _outputBuffer.concat(", ");
    _outputBuffer.concat(encoders[WHEEL_RIGHT].getCounter());
    _outputBuffer.concat("]");
    return 1;
}

int CommandProcessor::_commandStop() {
    _chassis.stop();
    return 0;
}

int CommandProcessor::_commandSetPwm() {
    int data[2];
    int index = 0;

    char myString[16];
    _inputBuffer.toCharArray(myString, sizeof(myString), 4);
    char *p = strtok(myString, ",");

    while (p) {
        if (index < 2) {
            data[index] = atoi(p);
        }
        p = strtok(NULL, " ");
        index++;
    }

    _chassis.drive(data[0], data[1]);
    return 0;
}

int CommandProcessor::_commandGetPwm() {
    Motor *motors = _chassis.getMotors();
    _outputBuffer.concat("[");
    _outputBuffer.concat(motors[WHEEL_LEFT].getPwm());
    _outputBuffer.concat(", ");
    _outputBuffer.concat(motors[WHEEL_RIGHT].getPwm());
    _outputBuffer.concat("]");
    return 1;
}

int CommandProcessor::_commandEnableDebug() {
    _debug = 1;
    _outputBuffer = "Debug set to 1";
    return 1;
}

int CommandProcessor::_commandDisableDebug() {
    _debug = 0;
    _outputBuffer = "Debug set to 0";
    return 1;
}

int CommandProcessor::_commandGetPerf() {
    _outputBuffer = "Performance = ";
    _outputBuffer.concat(_perf);
    return 1;
}

int CommandProcessor::_executeCommand() {
    _outputBuffer = "";

    if (_inputBuffer.equals("CHECK")) {
        return _commandCheck();
    }

    if (_inputBuffer.equals("RESET")) {
        return _commandReset();
    }

    if (_inputBuffer.equals("IRVAL?")) {
        return _commandGetIrVal();
    }

    if (_inputBuffer.equals("ENVEL?")) {
        return _commandGetEnVel();
    }

    if (_inputBuffer.equals("ENVAL?")) {
        return _commandGetEnVal();
    }

    if (_inputBuffer.equals("PWM=0,0")) {
        return _commandStop();
    }

    if (_inputBuffer.startsWith("PWM=")) {
        return _commandSetPwm();
    }

    if (_inputBuffer.equals("PWM?")) {
        return _commandGetPwm();
    }

    if (_inputBuffer.equals("DEBUG=1")) {
        return _commandEnableDebug();
    }

    if (_inputBuffer.equals("DEBUG=0")) {
        return _commandDisableDebug();
    }

    if (_inputBuffer.equals("PERF?")) {
        return _commandGetPerf();
    }

    _outputBuffer = "UNKNOWN COMMAND: ";
    _outputBuffer.concat(_inputBuffer);
    return 1;
}

void CommandProcessor::readCommand() {
    while (_stream.available()) {
        int incomingChar = _stream.read();
        if (incomingChar == -1) {
            continue;
        }

        if (incomingChar == '$') {
            _inputBuffer = "";
            continue;
        }

        if (incomingChar == '*') {
            int result = _executeCommand();
            if (result) {
                _stream.println(_outputBuffer);
            }
            continue;
        }

        _inputBuffer += (char)incomingChar;
    }
}
