#ifndef COMMANDS_CHECK_H
#define COMMANDS_CHECK_H

#include "Commands/Command.h"

class CheckCommand : public Command {
public:
    virtual int execute(String &input, String &output);
};

#endif
