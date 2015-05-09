#include "CommandGetPwm.h"

CommandGetPwm::CommandGetPwm(
    Chassis &chassis
) :
    _chassis(chassis)
{
}

int CommandGetPwm::execute(String &input, String &output) {
    Motor *motors = _chassis.getMotors();
    output.concat("[");
    output.concat(motors[WHEEL_LEFT].getPwm());
    output.concat(", ");
    output.concat(motors[WHEEL_RIGHT].getPwm());
    output.concat("]");
    return 1;
}
