#ifndef COMMANDRESET_H
#define COMMANDRESET_H

#include "Chassis.h"
#include "Command.h"

class CommandReset : public Command {
private:
    Chassis &_chassis;
public:
    CommandReset(Chassis &chassis);
    virtual int execute(String &input, String &output);
};

#endif
