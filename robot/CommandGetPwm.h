/**
 * Class for command: get pwm value from the motor drivers.
 *
 * Receives last set pwm values.
 * Correct values are from -255 to 255.
 *
 * COMMAND: $PWM?*
 * OUTPUT EXAMPLE: [155, 180]
 * OUTPUT FORMAT: [<int:left motor pwm value>, <int:right motor pwm value>]
 */

#ifndef COMMANDGETPWM_H
#define COMMANDGETPWM_H

#include "Chassis.h"
#include "Command.h"

class CommandGetPwm : public Command {
private:
    Chassis &_chassis;
public:
    CommandGetPwm(Chassis &chassis);
    virtual int execute(String &input, String &output);
};

#endif
