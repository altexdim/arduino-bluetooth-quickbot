#ifndef COMMANDDEBUGDISABLE_H
#define COMMANDDEBUGDISABLE_H

#include "Command.h"

class CommandDebugDisable : public Command {
private:
    int &_debug;
public:
    CommandDebugDisable(int &debug);
    virtual int execute(String &input, String &output);
};

#endif
