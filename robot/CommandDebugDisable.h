/**
 * Class for command: disable debug mode.
 *
 * Is been used for disabling debug output.
 *
 * COMMAND: $DEBUG=0*
 * OUTPUT FORMAT: <string>
 * OUTPUT: Debug set to 0
 */

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
