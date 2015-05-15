#include "commands/CommandSetPwm.h"

CommandSetPwm::CommandSetPwm(
    Chassis &chassis
) :
    _chassis(chassis)
{
}

int CommandSetPwm::execute(String &input, String &output) {
    int data[2];
    int index = 0;

    char myString[16];
    input.toCharArray(myString, sizeof(myString), 4);
    char *p = strtok(myString, ",");

    while (p) {
        if (index < 2) {
            data[index] = atoi(p);
        }
        p = strtok(NULL, " ");
        index++;
    }

    _chassis.drive(data[0], data[1]);
    return 0;
}
