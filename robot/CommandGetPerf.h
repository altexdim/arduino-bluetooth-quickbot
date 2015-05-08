#ifndef COMMANDGETPERF_H
#define COMMANDGETPERF_H

#include "Command.h"

class CommandGetPerf : public Command {
private:
    unsigned long &_perf;
public:
    CommandGetPerf(unsigned long &perf);
    virtual int execute(String &input, String &output);
};

#endif
