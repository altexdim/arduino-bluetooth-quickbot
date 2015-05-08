#ifndef COMMANDGETENVEL_H
#define COMMANDGETENVEL_H

#include "Chassis.h"
#include "Command.h"

class CommandGetEnVel : public Command {
private:
    Chassis &_chassis;
public:
    CommandGetEnVel(Chassis &chassis);
    virtual int execute(String &input, String &output);
};

#endif
