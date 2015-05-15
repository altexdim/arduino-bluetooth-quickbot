#include "commands/CommandGetPerf.h"

CommandGetPerf::CommandGetPerf(
    PerformanceCounter &perf
) :
    _perf(perf)
{
}

int CommandGetPerf::execute(String &input, String &output) {
    output = "Performance = ";
    output.concat(_perf.getPerformanceValue());
    return 1;
}
