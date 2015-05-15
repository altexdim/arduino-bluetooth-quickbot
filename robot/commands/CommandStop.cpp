#include "commands/CommandStop.h"

CommandStop::CommandStop(
    Chassis &chassis
) :
    _chassis(chassis)
{
}

int CommandStop::execute(String &input, String &output) {
    _chassis.stop();
    return 0;
}
