#include "Chassis.h"

Chassis::Chassis(
    Encoder *encoders,
    Motor *motors
) :
    _encoders(encoders),
    _motors(motors)
{
}

void Chassis::updateEncoders() {
    _encoders[WHEEL_LEFT].update();
    _encoders[WHEEL_RIGHT].update();
}

void Chassis::resetEncoders() {
    _encoders[WHEEL_LEFT].reset();
    _encoders[WHEEL_RIGHT].reset();
}

void Chassis::stop() {
    _motors[WHEEL_LEFT].stop(false);
    _motors[WHEEL_RIGHT].stop(false);
}

void Chassis::drive(int leftPwm, int rightPwm) {
    _motors[WHEEL_LEFT].drive(leftPwm);
    _encoders[WHEEL_LEFT].setDirection(_motors[WHEEL_LEFT].getPwm());
    _motors[WHEEL_RIGHT].drive(rightPwm);
    _encoders[WHEEL_RIGHT].setDirection(_motors[WHEEL_RIGHT].getPwm());
}

Encoder *Chassis::getEncoders() {
    return _encoders;
}

Motor *Chassis::getMotors() {
    return _motors;
}
