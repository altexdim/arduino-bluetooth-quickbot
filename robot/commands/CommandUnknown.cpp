#include "commands/CommandUnknown.h"

int CommandUnknown::execute(String &input, String &output) {
    output = "UNKNOWN COMMAND: ";
    output.concat(input);
    return 1;
}
