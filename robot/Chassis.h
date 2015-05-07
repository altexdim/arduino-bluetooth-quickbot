#ifndef CHASSIS_H
#define CHASSIS_H

#include "Encoder.h"
#include "Motor.h"
#include "Types.h"

class Chassis {
private:
    Encoder *_encoders;
    Motor *_motors;

public:
    Chassis(
        Encoder *encoders,
        Motor *motors
    );
    void resetEncoders();
    void stop();
    void drive(int leftPwm, int rightPwm);
    Encoder *getEncoders();
    Motor *getMotors();
};

#endif
