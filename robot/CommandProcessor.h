/**
 * Command dispatcher, decoder and executor.
 *
 * Reads commands from stream.
 * Detects and parse commands from string.
 * Execute appropriate detected commands.
 * Returns command output to stream.
 */

#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include "Command.h"
#include "Arduino.h"
#include "Chassis.h"
#include "IrSensorsCollection.h"
#include "Types.h"

class CommandProcessor {
private:
    Command **_commands;
    // Input string buffer
    String _inputBuffer;
    // Output stream buffer
    String _outputBuffer;
    // Stream to read commands and to write output.
    Stream &_stream;

    // Chassis of the robot
    Chassis &_chassis;
    // IR sensors collection
    IrSensorsCollection &_sensorsCollection;

    // Debug mode
    int &_debug;
    // Performance counter
    unsigned long &_perf;

    /**
     * Parse and execute command.
     *
     * Used for commands dispatching.
     */
    COMMAND _decodeCommand();

    /**
     * Drive motors.
     */
    int _commandSetPwm();
    /**
     * Get last pwm motor driver values.
     */
    int _commandGetPwm();
    /**
     * Enable debug mode command.
     */
    int _commandEnableDebug();
    /**
     * Disable debug mode command.
     */
    int _commandDisableDebug();
    /**
     * Get performance counter value.
     */
    int _commandGetPerf();

public:
    /**
     * Constructor
     *
     * @param Strem stream - Input and output stream.
     * @param Chassis chassis - Robot chassis with encoders and motors.
     * @param IrSensorsCollection sensorsCollection - IR sensors collection.
     * @param int debug - Debug mode flag
     * @param long perf - Performance counter
     */
    CommandProcessor(
        Command **commands,
        Stream &stream,
        Chassis &chassis,
        IrSensorsCollection &sensorsCollection,
        int &debug,
        unsigned long &perf
    );
    /**
     * Read command from stream and execute it.
     */
    void readCommand();
};

#endif
