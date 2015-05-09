/**
 * Class for command: receive encoders velocity.
 *
 * Returns current encoders velocity
 * for left and right encoders respectively.
 *
 * Velocity in ticks per second.
 *
 * COMMAND: $ENVEL?*
 * OUTPUT EXAMPLE: [-15, 20]
 * OUTPUT FORMAT: [<int:left encoder velocity>, <int:right encoder velocity>]
 */

#ifndef COMMANDGETENVEL_H
#define COMMANDGETENVEL_H

#include "Chassis.h"
#include "Command.h"

class CommandGetEnVel : public Command {
private:
    Chassis &_chassis;
public:
    CommandGetEnVel(Chassis &chassis);
    virtual int execute(String &input, String &output);
};

#endif
