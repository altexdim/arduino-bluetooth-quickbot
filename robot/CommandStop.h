#ifndef COMMANDSTOP_H
#define COMMANDSTOP_H

#include "Chassis.h"
#include "Command.h"

class CommandStop : public Command {
private:
    Chassis &_chassis;
public:
    CommandStop(Chassis &chassis);
    virtual int execute(String &input, String &output);
};

#endif
