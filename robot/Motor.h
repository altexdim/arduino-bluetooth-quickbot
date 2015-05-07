#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

#define MOTOR_PWM_MAX_VALUE 255

class Motor {
private:
    unsigned int _pinEn1;
    unsigned int _pinEn2;
    unsigned int _pinPwm;
    int _pwm;
public:
    Motor(unsigned int pinEn1, unsigned int pinEn2, unsigned int pinPwm);
    void init();
    void drive(int spd);
    void stop(bool hard = false);
    int getPwm();
};

#endif
