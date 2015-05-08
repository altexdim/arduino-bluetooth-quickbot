#ifndef COMMANDDEBUGENABLE_H
#define COMMANDDEBUGENABLE_H

#include "Command.h"

class CommandDebugEnable : public Command {
private:
    int &_debug;
public:
    CommandDebugEnable(int &debug);
    virtual int execute(String &input, String &output);
};

#endif
