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
    COMMAND_COUNT
};

#endif
