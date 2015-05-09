#ifndef COMMANDDEBUGENABLE_H
#define COMMANDDEBUGENABLE_H

#include "Command.h"
#include "Debugger.h"

class CommandDebugEnable : public Command {
private:
    Debugger &_debugger;
public:
    CommandDebugEnable(Debugger &debugger);
    virtual int execute(String &input, String &output);
};

#endif
