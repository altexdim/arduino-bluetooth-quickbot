#include "CommandProcessor.h"

CommandProcessor::CommandProcessor(
    Command **commands,
    Stream &stream
) :
    _commands(commands),
    _stream(stream)
{
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

    if (_inputBuffer.equals("PWM?")) {
        return COMMAND_GETPWM;
    }

    if (_inputBuffer.equals("DEBUG=1")) {
        return COMMAND_DEBUG_ENABLE;
    }

    if (_inputBuffer.equals("DEBUG=0")) {
        return COMMAND_DEBUG_DISABLE;
    }

    if (_inputBuffer.equals("PERF?")) {
        return COMMAND_GETPERF;
    }

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
