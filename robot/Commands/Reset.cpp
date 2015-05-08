#include "Commands/Reset.h"

ResetCommand::ResetCommand(
    Chassis &chassis
) :
    _chassis(chassis)
{
}

int ResetCommand::execute(String &input, String &output) {
    _chassis.resetEncoders();
    output = "Encoder values reset to [0, 0]";
    return 1;
}
