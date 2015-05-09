/**
 * Class for command: enable debug mode.
 *
 * Is been used for enabling debug output.
 *
 * COMMAND: $DEBUG=1*
 * OUTPUT FORMAT: <string>
 * OUTPUT: Debug set to 1
 */

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
