#ifndef COMMANDS_GETIRVAL_H
#define COMMANDS_GETIRVAL_H

#include "IrSensorsCollection.h"
#include "Commands/Command.h"

class GetIrValCommand : public Command {
private:
    IrSensorsCollection &_sensorsCollection;
public:
    GetIrValCommand(IrSensorsCollection &sensorsCollection);
    virtual int execute(String &input, String &output);
};

#endif
