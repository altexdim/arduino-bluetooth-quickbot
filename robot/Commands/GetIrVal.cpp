#include "Commands/GetIrVal.h"

GetIrValCommand::GetIrValCommand(
    IrSensorsCollection &sensorsCollection
) :
    _sensorsCollection(sensorsCollection)
{
}

int GetIrValCommand::execute(String &input, String &output) {
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
