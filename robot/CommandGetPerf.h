/**
 * Class for command: receive performance counter value.
 *
 * Returns current value of the performance counter.
 *
 * In program loops per second.
 *
 * COMMAND: $PERF?*
 * OUTPUT EXAMPLE: "Performance = 1850"
 * OUTPUT FORMAT: <string>
 */

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
