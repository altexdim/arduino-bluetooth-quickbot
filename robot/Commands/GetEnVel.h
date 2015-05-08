#ifndef COMMANDS_GETENVEL_H
#define COMMANDS_GETENVEL_H

#include "Chassis.h"
#include "Commands/Command.h"

class GetEnVelCommand : public Command {
private:
    Chassis &_chassis;
public:
    GetEnVelCommand(Chassis &chassis);
    virtual int execute(String &input, String &output);
};

#endif
