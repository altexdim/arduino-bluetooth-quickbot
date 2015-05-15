/**
 * Command dispatcher, decoder and executor.
 *
 * Reads commands from stream.
 * Detects and parse commands from string.
 * Execute appropriate detected commands.
 * Returns command output to stream.
 */

#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include "commands/Command.h"
#include "Arduino.h"
#include "Types.h"

class CommandProcessor {
private:
    // Array of the available commands.
    Command **_commands;
    // Input string buffer
    String _inputBuffer;
    // Output stream buffer
    String _outputBuffer;
    // Stream to read commands and to write output.
    Stream &_stream;

    /**
     * Parse input buffer and detect command.
     *
     * @return COMMAND - Returns detected command. Returns COMMAND_UNKNOWN if failed.
     */
    COMMAND _decodeCommand();

public:
    /**
     * Constructor
     *
     * @param Command commands - Array of commands (pointer to array of pointers of objects with Command interface).
     * @param Strem stream - Input and output stream.
     */
    CommandProcessor(
        Command **commands,
        Stream &stream
    );
    /**
     * Read command from stream and execute it.
     */
    void readCommand();
};

#endif
