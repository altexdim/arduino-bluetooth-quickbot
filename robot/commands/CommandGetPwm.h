/**
 * Class for command: get pwm value from the motor drivers.
 *
 * Receives last set pwm values.
 * Correct values are from -100 to 100.
 *
 * COMMAND: $PWM?*
 * OUTPUT EXAMPLE: [55, 80]
 * OUTPUT FORMAT: [<int:left motor pwm value>, <int:right motor pwm value>]
 */

#ifndef COMMANDGETPWM_H
#define COMMANDGETPWM_H

#include "Chassis.h"
#include "commands/Command.h"

class CommandGetPwm : public Command {
private:
    Chassis &_chassis;
public:
    CommandGetPwm(Chassis &chassis);
    virtual int execute(String &input, String &output);
};

#endif
