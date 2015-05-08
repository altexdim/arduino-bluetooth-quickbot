#ifndef COMMAND_H
#define COMMAND_H

#include <Arduino.h>

class Command {
public:
    virtual int execute(String &input, String &output) = 0;
};

#endif
