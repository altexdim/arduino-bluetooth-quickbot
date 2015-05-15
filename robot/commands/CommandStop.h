/**
 * Class for command: stop motors.
 *
 * Makes motors stop.
 *
 * COMMAND: $PWM=0,0*
 * OUTPUT FORMAT: None
 */

#ifndef COMMANDSTOP_H
#define COMMANDSTOP_H

#include "Chassis.h"
#include "commands/Command.h"

class CommandStop : public Command {
private:
    Chassis &_chassis;
public:
    CommandStop(Chassis &chassis);
    virtual int execute(String &input, String &output);
};

#endif
