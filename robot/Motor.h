/**
 * Class for controlling motors.
 *
 * Provides high-level interface to manage motor drivers.
 * Incapsulates low-level functions like GPIO management.
 */

#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

// Maximum PWM value to use in arguments of drive() method.
#define MOTOR_PWM_MAX_VALUE 100
// Internal maximum PWM value to use in analorWrite() function.
#define MOTOR_PORT_PWM_MAX_VALUE 255

class Motor {
private:
    // Direction pin1 of the motor driver
    unsigned int _pinEn1;
    // Direction pin2 of the motor driver
    unsigned int _pinEn2;
    // Pwm pin of the motor driver
    unsigned int _pinPwm;
    // Pwm value last written to the pwm port
    int _pwm;
public:
    /**
     * Constructor
     *
     * @param int pinEn1 - Direction pin1
     * @param int pinEn2 - Direction pin2
     * @param int pinPwm - Pwm pin
     */
    Motor(unsigned int pinEn1, unsigned int pinEn2, unsigned int pinPwm);
    /**
     * Initialize GPIO pins
     */
    void init();
    /**
     * Drive motor
     *
     * Negative speed means backward direction.
     * Speed is limited from -MOTOR_PWM_MAX_VALUE to MOTOR_PWM_MAX_VALUE.
     *
     * @param int spd - Speed of motor.
     */
    void drive(int spd);
    /**
     * Stop motor
     *
     * Hard stop means actively block motor
     * without inertion rotation.
     *
     * @param bool hard - Hard stop. Default false.
     */
    void stop(bool hard = false);
    /**
     * Get last set pwm value
     */
    int getPwm();
};

#endif
