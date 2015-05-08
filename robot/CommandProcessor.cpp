#include "CommandProcessor.h"

CommandProcessor::CommandProcessor(
    Command **commands,
    Stream &stream,
    Chassis &chassis,
    IrSensorsCollection &sensorsCollection,
    int &debug,
    unsigned long &perf
) :
    _commands(commands),
    _stream(stream),
    _chassis(chassis),
    _sensorsCollection(sensorsCollection),
    _debug(debug),
    _perf(perf)
{
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

COMMAND CommandProcessor::_decodeCommand() {
    _outputBuffer = "";

    if (_inputBuffer.equals("CHECK")) {
        return COMMAND_CHECK;
    }

    if (_inputBuffer.equals("RESET")) {
        return COMMAND_RESET;
    }

    if (_inputBuffer.equals("IRVAL?")) {
        return COMMAND_GETIRVAL;
    }

    if (_inputBuffer.equals("ENVEL?")) {
        return COMMAND_GETENVEL;
    }

    if (_inputBuffer.equals("ENVAL?")) {
        return COMMAND_GETENVAL;
    }

    if (_inputBuffer.equals("PWM=0,0")) {
        return COMMAND_STOP;
    }

    if (_inputBuffer.startsWith("PWM=")) {
        return COMMAND_SETPWM;
    }

//    if (_inputBuffer.equals("PWM?")) {
//        return _commandGetPwm();
//    }
//
//    if (_inputBuffer.equals("DEBUG=1")) {
//        return _commandEnableDebug();
//    }
//
//    if (_inputBuffer.equals("DEBUG=0")) {
//        return _commandDisableDebug();
//    }
//
//    if (_inputBuffer.equals("PERF?")) {
//        return _commandGetPerf();
//    }
    return COMMAND_UNKNOWN;
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
            COMMAND decodedCommand = _decodeCommand();
            int result = _commands[decodedCommand]->execute(_inputBuffer, _outputBuffer);
            if (result) {
                _stream.println(_outputBuffer);
            }
            continue;
        }

        _inputBuffer += (char)incomingChar;
    }
}
