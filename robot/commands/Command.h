/**
 * Command interface.
 */

#ifndef COMMAND_H
#define COMMAND_H

#include <Arduino.h>

class Command {
public:
    /**
     * Executes command.
     *
     * @param String input - Input string.
     * @param String output - Output string.
     * @return int - "1" is for send output string to the client, "0" means command has been executed silently.
     */
    virtual int execute(String &input, String &output) = 0;
};

#endif
