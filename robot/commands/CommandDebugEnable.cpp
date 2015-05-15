#include "commands/CommandDebugEnable.h"

CommandDebugEnable::CommandDebugEnable(
    Debugger &debugger
) :
    _debugger(debugger)
{
}

int CommandDebugEnable::execute(String &input, String &output) {
    _debugger.setDebugMode(1);
    output = "Debug set to 1";
    return 1;
}
