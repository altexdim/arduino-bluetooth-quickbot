#include "commands/CommandGetEnVal.h"

CommandGetEnVal::CommandGetEnVal(
    Chassis &chassis
) :
    _chassis(chassis)
{
}

int CommandGetEnVal::execute(String &input, String &output) {
    Encoder *encoders = _chassis.getEncoders();
    output.concat("[");
    output.concat(encoders[WHEEL_LEFT].getCounter());
    output.concat(", ");
    output.concat(encoders[WHEEL_RIGHT].getCounter());
    output.concat("]");
    return 1;
}
