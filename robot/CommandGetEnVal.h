/**
 * Class for receiving encoders value.
 *
 * Returns current encoders value
 * for left and right encoders respectively.
 *
 * COMMAND: $ENVAL?*
 * OUTPUT EXAMPLE: [-5, 10]
 * OUTPUT FORMAT: [<int:left encoder value>, <int:right encoder value>]
 */

#ifndef COMMANDGETENVAL_H
#define COMMANDGETENVAL_H

#include "Chassis.h"
#include "Command.h"

class CommandGetEnVal : public Command {
private:
    Chassis &_chassis;
public:
    CommandGetEnVal(Chassis &chassis);
    virtual int execute(String &input, String &output);
};

#endif
