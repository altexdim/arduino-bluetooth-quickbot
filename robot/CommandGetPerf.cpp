#include "CommandGetPerf.h"

CommandGetPerf::CommandGetPerf(
    unsigned long &perf
) :
    _perf(perf)
{
}

int CommandGetPerf::execute(String &input, String &output) {
    output = "Performance = ";
    output.concat(_perf);
    return 1;
}
