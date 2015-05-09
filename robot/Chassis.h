/**
 * Class represents robot chassis.
 *
 * Contains encoders and motors. When you set pwm to the motors, it set encoder direction as well.
 */

#ifndef CHASSIS_H
#define CHASSIS_H

#include "Encoder.h"
#include "Motor.h"
#include "Types.h"

class Chassis {
private:
    // Encoders array
    Encoder *_encoders;
    // Motors array
    Motor *_motors;

public:
    /**
     * Constructor
     *
     * @param Encoder encoders - Encoders array
     * @param Motor motors - Motors array
     */
    Chassis(
        Encoder *encoders,
        Motor *motors
    );
    void updateEncoders();
    /**
     * Resets encoders to 0.
     */
    void resetEncoders();
    /**
     * Stops motors
     */
    void stop();
    /**
     * Starts motors and sets encoders direction
     */
    void drive(int leftPwm, int rightPwm);
    /**
     * Returns encoders array
     */
    Encoder *getEncoders();
    /**
     * Returns motors array
     */
    Motor *getMotors();
};

#endif
