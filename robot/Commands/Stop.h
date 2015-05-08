#ifndef COMMANDS_STOP_H
#define COMMANDS_STOP_H

#include "Chassis.h"
#include "Commands/Command.h"

class StopCommand : public Command {
private:
    Chassis &_chassis;
public:
    StopCommand(Chassis &chassis);
    virtual int execute(String &input, String &output);
};

#endif
