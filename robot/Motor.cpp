#include "Motor.h"

Motor::Motor(
    unsigned int pinEn1,
    unsigned int pinEn2,
    unsigned int pinPwm
) :
    _pinEn1(pinEn1),
    _pinEn2(pinEn2),
    _pinPwm(pinPwm)
{
    _pwm = 0;
}

void Motor::init() {
    pinMode(_pinEn1, OUTPUT);
    digitalWrite(_pinEn1, LOW);
    pinMode(_pinEn2, OUTPUT);
    digitalWrite(_pinEn2, LOW);
    pinMode(_pinPwm, OUTPUT);
    digitalWrite(_pinPwm, LOW);
}

void Motor::drive(int spd) {
    if (spd < -MOTOR_PWM_MAX_VALUE || spd > MOTOR_PWM_MAX_VALUE) {
        return;
    }

    _pwm = spd;
    int pwmValue = (int)map(_pwm, -MOTOR_PWM_MAX_VALUE, MOTOR_PWM_MAX_VALUE, -MOTOR_PORT_PWM_MAX_VALUE, MOTOR_PORT_PWM_MAX_VALUE);
    if (_pwm < 0) {
        digitalWrite(_pinEn1, LOW);
        digitalWrite(_pinEn2, HIGH);
        analogWrite(_pinPwm, -pwmValue);
    } else {
        digitalWrite(_pinEn1, HIGH);
        digitalWrite(_pinEn2, LOW);
        analogWrite(_pinPwm, pwmValue);
    }
}

void Motor::stop(bool hard) {
    _pwm = 0;
    analogWrite(_pinPwm, 0);
    digitalWrite(_pinEn1, hard? HIGH: LOW);
    digitalWrite(_pinEn2, hard? HIGH: LOW);
}

int Motor::getPwm() {
    return _pwm;
}
