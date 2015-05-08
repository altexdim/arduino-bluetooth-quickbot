#ifndef COMMANDS_RESET_H
#define COMMANDS_RESET_H

#include "Chassis.h"
#include "Commands/Command.h"

class ResetCommand : public Command {
private:
    Chassis &_chassis;
public:
    ResetCommand(Chassis &chassis);
    virtual int execute(String &input, String &output);
};

#endif
