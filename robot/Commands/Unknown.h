#ifndef COMMANDS_UNKNOWN_H
#define COMMANDS_UNKNOWN_H

#include "Commands/Command.h"

class UnknownCommand : public Command {
public:
    virtual int execute(String &input, String &output);
};

#endif
