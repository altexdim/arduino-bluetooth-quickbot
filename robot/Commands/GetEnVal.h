#ifndef COMMANDS_GETENVAL_H
#define COMMANDS_GETENVAL_H

#include "Chassis.h"
#include "Commands/Command.h"

class GetEnValCommand : public Command {
private:
    Chassis &_chassis;
public:
    GetEnValCommand(Chassis &chassis);
    virtual int execute(String &input, String &output);
};

#endif
