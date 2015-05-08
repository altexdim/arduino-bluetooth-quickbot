#ifndef COMMANDGETENVAL_H
#define COMMANDGETENVAL_H

#include "Chassis.h"
#include "Command.h"

class CommandGetEnVal : public Command {
private:
    Chassis &_chassis;
public:
    CommandGetEnVal(Chassis &chassis);
    virtual int execute(String &input, String &output);
};

#endif
