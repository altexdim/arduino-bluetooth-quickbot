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
