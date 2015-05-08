#include "CommandGetEnVal.h"

CommandGetEnVal::CommandGetEnVal(
    Chassis &chassis
) :
    _chassis(chassis)
{
}

int CommandGetEnVal::execute(String &input, String &output) {
    Encoder *encoders = _chassis.getEncoders();
    output.concat("[");
    output.concat(encoders[WHEEL_LEFT].getVelocity());
    output.concat(", ");
    output.concat(encoders[WHEEL_RIGHT].getVelocity());
    output.concat("]");
    return 1;
}
