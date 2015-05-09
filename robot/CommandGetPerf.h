#ifndef COMMANDGETPERF_H
#define COMMANDGETPERF_H

#include "Command.h"
#include "PerformanceCounter.h"

class CommandGetPerf : public Command {
private:
    PerformanceCounter &_perf;
public:
    CommandGetPerf(PerformanceCounter &perf);
    virtual int execute(String &input, String &output);
};

#endif
