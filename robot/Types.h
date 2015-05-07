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
 * Pins for each motor driver
 */
enum WHEEL_PINS {
    // Direction pin 1
    WHEEL_PIN_EN1,
    // Direction pin 2
    WHEEL_PIN_EN2,
    // Pwm pin
    WHEEL_PIN_PWM,
    // Total number of pins
    WHEEL_PIN_COUNT
};

#endif
