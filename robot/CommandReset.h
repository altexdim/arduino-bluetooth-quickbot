/**
 * Class for command: reset encoders value and velocity.
 *
 * Resets encoder value and velocity to zero.
 *
 * COMMAND: $RESET*
 * OUTPUT EXAMPLE: Encoder values reset to [0, 0]
 * OUTPUT FORMAT: <string>
 */

#ifndef COMMANDRESET_H
#define COMMANDRESET_H

#include "Chassis.h"
#include "Command.h"

class CommandReset : public Command {
private:
    Chassis &_chassis;
public:
    CommandReset(Chassis &chassis);
    virtual int execute(String &input, String &output);
};

#endif
