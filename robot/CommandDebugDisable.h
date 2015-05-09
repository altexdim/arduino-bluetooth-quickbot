#ifndef COMMANDDEBUGDISABLE_H
#define COMMANDDEBUGDISABLE_H

#include "Command.h"
#include "Debugger.h"

class CommandDebugDisable : public Command {
private:
    Debugger &_debugger;
public:
    CommandDebugDisable(Debugger &debugger);
    virtual int execute(String &input, String &output);
};

#endif
