#include "CommandDebugDisable.h"

CommandDebugDisable::CommandDebugDisable(
    int &debug
) :
    _debug(debug)
{
}

int CommandDebugDisable::execute(String &input, String &output) {
    _debug = 0;
    output = "Debug set to 0";
    return 1;
}
