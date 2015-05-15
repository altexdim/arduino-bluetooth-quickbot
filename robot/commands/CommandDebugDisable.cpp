#include "commands/CommandDebugDisable.h"

CommandDebugDisable::CommandDebugDisable(
    Debugger &debugger
) :
    _debugger(debugger)
{
}

int CommandDebugDisable::execute(String &input, String &output) {
    _debugger.setDebugMode(0);
    output = "Debug set to 0";
    return 1;
}
