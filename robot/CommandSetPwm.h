#ifndef COMMANDSETPWM_H
#define COMMANDSETPWM_H

#include "Chassis.h"
#include "Command.h"

class CommandSetPwm : public Command {
private:
    Chassis &_chassis;
public:
    CommandSetPwm(Chassis &chassis);
    virtual int execute(String &input, String &output);
};

#endif
