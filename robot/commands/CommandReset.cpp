#include "commands/CommandReset.h"

CommandReset::CommandReset(
    Chassis &chassis
) :
    _chassis(chassis)
{
}

int CommandReset::execute(String &input, String &output) {
    _chassis.resetEncoders();
    output = "Encoder values reset to [0, 0]";
    return 1;
}
