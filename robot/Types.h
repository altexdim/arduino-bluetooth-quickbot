/**
 * Some usefull enums and structs.
 */

#ifndef TYPES_H
#define TYPES_H

/**
 * Wheels enumeration for motors and encoders
 */
enum WHEEL {
    // Left wheel
    WHEEL_LEFT,
    // Right wheel
    WHEEL_RIGHT,
    // Total number of wheels
    WHEEL_COUNT
};

/**
 * Commands
 */
enum COMMAND {
    // $CHECK*
    COMMAND_CHECK,
    // $RESET*
    COMMAND_RESET,
    // Any undeclared command
    COMMAND_UNKNOWN,
    // $IRVAL?*
    COMMAND_GETIRVAL,
    // $ENVAL?*
    COMMAND_GETENVAL,
    // $ENVEL?*
    COMMAND_GETENVEL,
    // $PWM=0,0*
    COMMAND_STOP,
    // $PWM=X,Y*
    COMMAND_SETPWM,
    // $PWM?*
    COMMAND_GETPWM,
    // $DEBUG=1*
    COMMAND_DEBUG_ENABLE,
    // $DEBUG=0*
    COMMAND_DEBUG_DISABLE,
    // $PERF?*
    COMMAND_GETPERF,
    // Total number of commands
    COMMAND_COUNT
};

#endif
