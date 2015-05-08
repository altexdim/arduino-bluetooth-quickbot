#ifndef COMMANDGETPWM_H
#define COMMANDGETPWM_H

#include "Chassis.h"
#include "Command.h"

class CommandGetPwm : public Command {
private:
    Chassis &_chassis;
public:
    CommandGetPwm(Chassis &chassis);
    virtual int execute(String &input, String &output);
};

#endif
