/**
 * Class for command: unknown or undetected command.
 *
 * Executes if can not detect command.
 *
 * COMMAND: $abcd*
 * OUTPUT FORMAT: <string>
 * OUTPUT EXAMPLE: "UNKNOWN COMMAND: abcd"
 */

#ifndef COMMANDUNKNOWN_H
#define COMMANDUNKNOWN_H

#include "commands/Command.h"

class CommandUnknown : public Command {
public:
    virtual int execute(String &input, String &output);
};

#endif
