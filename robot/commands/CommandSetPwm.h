/**
 * Class for command: set pwm value to the motor drivers.
 *
 * Makes motors move.
 * Valid values are from -100 to 100.
 *
 * COMMAND: $PWM=12,23*
 * INPUT FORMAT: $PWM=<int:left motor pwm value>,<int:right motor pwm value>*
 * OUTPUT FORMAT: None
 */

#ifndef COMMANDSETPWM_H
#define COMMANDSETPWM_H

#include "Chassis.h"
#include "commands/Command.h"

class CommandSetPwm : public Command {
private:
    Chassis &_chassis;
public:
    CommandSetPwm(Chassis &chassis);
    virtual int execute(String &input, String &output);
};

#endif
