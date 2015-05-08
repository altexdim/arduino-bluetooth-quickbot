#include "Commands/Unknown.h"

int UnknownCommand::execute(String &input, String &output) {
    output = "UNKNOWN COMMAND: ";
    output.concat(input);
    return 1;
}
