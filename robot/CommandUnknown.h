#ifndef COMMANDUNKNOWN_H
#define COMMANDUNKNOWN_H

#include "Command.h"

class CommandUnknown : public Command {
public:
    virtual int execute(String &input, String &output);
};

#endif
