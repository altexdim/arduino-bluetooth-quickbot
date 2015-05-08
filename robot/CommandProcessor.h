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

#include "Arduino.h"
#include "Chassis.h"
#include "IrSensorsCollection.h"

class CommandProcessor {
private:
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
    int _executeCommand();


    /**
     * Check command.
     *
     * Used for ping.
     */
    int _commandCheck();
    /**
     * Reset command.
     *
     * Resets encoder counters.
     */
    int _commandReset();
    /**
     * Get IR sensor values.
     */
    int _commandGetIrVal();
    /**
     * Get encoders velocity.
     */
    int _commandGetEnVel();
    /**
     * Get encoders counters.
     */
    int _commandGetEnVal();
    /**
     * Stop motors.
     */
    int _commandStop();
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
