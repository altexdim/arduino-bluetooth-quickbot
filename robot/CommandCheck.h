/**
 * Class for command: Check (Ping).
 *
 * Is been used for ping.
 *
 * COMMAND: $CHECK?*
 * OUTPUT FORMAT: <string>
 * OUTPUT EXAMPLE: Hello from QuickBot
 */

#ifndef COMMANDCHECK_H
#define COMMANDCHECK_H

#include "Command.h"

class CommandCheck : public Command {
public:
    virtual int execute(String &input, String &output);
};

#endif
