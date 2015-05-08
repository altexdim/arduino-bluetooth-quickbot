#include "Commands/Check.h"

CheckCommand():
{
}

int CheckCommand::execute(String &input, String &output) {
    output = "Hello from QuickBot";
    return 1;
}
