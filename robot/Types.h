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

enum COMMAND {
    COMMAND_CHECK,
    COMMAND_RESET,
    COMMAND_UNKNOWN,
    COMMAND_GETIRVAL,
    COMMAND_GETENVAL,
    COMMAND_GETENVEL,
    COMMAND_STOP,
    COMMAND_SETPWM,
    COMMAND_COUNT
};

#endif
