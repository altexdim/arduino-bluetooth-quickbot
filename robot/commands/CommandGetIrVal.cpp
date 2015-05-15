#include "commands/CommandGetIrVal.h"

CommandGetIrVal::CommandGetIrVal(
    IrSensorsCollection &sensorsCollection
) :
    _sensorsCollection(sensorsCollection)
{
}

int CommandGetIrVal::execute(String &input, String &output) {
    output.concat("[");
    IrSensor *sensors = _sensorsCollection.getSensors();
    unsigned int count = _sensorsCollection.count();
    for (int i = 0; i < count; i++) {
        if (i) {
            output.concat(", ");
        }
        output.concat(sensors[i].getDistance());
    }
    output.concat("]");
    return 1;
}
