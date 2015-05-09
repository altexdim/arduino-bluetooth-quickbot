/**
 * Class for receiving infrared sensors values command.
 *
 * Returns current value
 * for all five sensors.
 *
 * COMMAND: $IRVAL?*
 * OUTPUT EXAMPLE: [100, 150, 200, 250, 300]
 * OUTPUT FORMAT: [<int:N-th sensor value>, ...]
 */

#ifndef COMMANDGETIRVAL_H
#define COMMANDGETIRVAL_H

#include "IrSensorsCollection.h"
#include "Command.h"

class CommandGetIrVal : public Command {
private:
    IrSensorsCollection &_sensorsCollection;
public:
    CommandGetIrVal(IrSensorsCollection &sensorsCollection);
    virtual int execute(String &input, String &output);
};

#endif
