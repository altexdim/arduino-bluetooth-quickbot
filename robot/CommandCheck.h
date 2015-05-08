#ifndef COMMANDCHECK_H
#define COMMANDCHECK_H

#include "Command.h"

class CommandCheck : public Command {
public:
    virtual int execute(String &input, String &output);
};

#endif
