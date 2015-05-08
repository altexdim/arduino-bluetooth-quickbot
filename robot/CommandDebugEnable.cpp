#include "CommandDebugEnable.h"

CommandDebugEnable::CommandDebugEnable(
    int &debug
) :
    _debug(debug)
{
}

int CommandDebugEnable::execute(String &input, String &output) {
    _debug = 1;
    output = "Debug set to 1";
    return 1;
}
