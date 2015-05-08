#include "Commands/Stop.h"

StopCommand::StopCommand(
    Chassis &chassis
) :
    _chassis(chassis)
{
}

int StopCommand::execute(String &input, String &output) {
    _chassis.stop();
    return 0;
}
