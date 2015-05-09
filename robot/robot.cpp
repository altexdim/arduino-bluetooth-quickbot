/**
 * This is fully functional QuickBot implementation on the Arduino Platform.
 * Original QuickBot site and repository http://o-botics.org/robots/quickbot/mooc/v2/
 * QuickBot originated from coursera course "Control of Mobile Robots": 
 * https://www.coursera.org/course/conrob
 * 
 * This is early alfa release. But it fully functional.
 *
 * It can
 *     - read values from 5 IR sensors
 *     - calculate wheel rotations with encoders with precision of 1/16 wheel turn
 *     - manage the velocity of the motors
 *     - read and execute commands via bluetooth module (bluetooth is connected to HW serial port)
 *     - send data and command results back to supervisor via bluetooth connection (serial port)
 *
 * ==============================================================================================
 *
 * I use lowpass filter for the encoder readings. Encoders are connected to the analog input ports.
 * Low pass filter takes 3 values and calculates average value.
 *
 * Also there is a filter for IR sensors, it takes 3 readings from IR sensor and calculates average.
 *
 * Debug mode is supported. Debug commands are:
 *     - $DEBUG=1* - enable debug mode
 *     - $DEBUG=0* - disable debug mode
 *     - $PERF?* - show performance average
 *
 * $PERF?* command shows how many loops does take place in one second.
 * If the program fast enough then you can see more than 1000 loops (1800-1900 loops is very good value).
 * 1000 loops per second means that each loop is as fast as 1 millisecond.
 * 
 * Motors can be started for one second with pressing the button connected to pin 12.
 *
 * ==============================================================================================
 *
 * Some statistics.
 *
 * Before refactoring the program had
 *      Firmware Size:  [Program:  bytes (.%)]  [Data:  bytes (.%)] on atmega328p
 *      EEPROM   Size:  [Program: 0 bytes (0.0%)]  [Data: 0 bytes (0.0%)] on atmega328p
 *      Performance counter = 1918
 * After refactoring the program has
 *      Firmware Size:  [Program: 10466 bytes (31.9%)]  [Data: 760 bytes (37.1%)] on atmega328p
 *      EEPROM   Size:  [Program: 0 bytes (0.0%)]  [Data: 0 bytes (0.0%)] on atmega328p
 *      Performance counter = 1827
 *
 * ==============================================================================================
 *
 * Issues:
 *      1) How to put all commands files into subdirectory? Cmake does not compile them.
 *      The only workaround is to #include both .h and .cpp files, but that's not i want.
 *
 */

#include <Arduino.h>
#include "Encoder.h"
#include "Types.h"
#include "Settings.h"
#include "IrSensor.h"
#include "IrSensorsCollection.h"
#include "Motor.h"
#include "Chassis.h"
#include "PerformanceCounter.h"
#include "Debugger.h"
#include "CommandProcessor.h"
#include "CommandCheck.h"
#include "CommandReset.h"
#include "CommandUnknown.h"
#include "CommandGetIrVal.h"
#include "CommandGetEnVal.h"
#include "CommandGetEnVel.h"
#include "CommandStop.h"
#include "CommandSetPwm.h"
#include "CommandGetPwm.h"
#include "CommandDebugEnable.h"
#include "CommandDebugDisable.h"
#include "CommandGetPerf.h"

// Encoders
Encoder encoders[WHEEL_COUNT] = {
    Encoder(LEFT_WHEEL_ENCODER_PIN),
    Encoder(RIGHT_WHEEL_ENCODER_PIN),
};

// IR proximity sensors
IrSensor sensors[IR_SENSORS_COUNT] = {
    IrSensor(IR_SENSOR_1_PIN),
    IrSensor(IR_SENSOR_2_PIN),
    IrSensor(IR_SENSOR_3_PIN),
    IrSensor(IR_SENSOR_4_PIN),
    IrSensor(IR_SENSOR_5_PIN),
};

// IR sensors collection, update manager
IrSensorsCollection sensorsCollection(sensors, IR_SENSORS_COUNT);

// Motors
Motor motors[WHEEL_COUNT] = {
    Motor(LEFT_MOTOR_EN1_PIN, LEFT_MOTOR_EN2_PIN, LEFT_MOTOR_PWM_PIN),
    Motor(RIGHT_MOTOR_EN1_PIN, RIGHT_MOTOR_EN2_PIN, RIGHT_MOTOR_PWM_PIN),
};

// Chassis
Chassis chassis(encoders, motors);

// Performance counter. Counts program loops per second.
PerformanceCounter performanceCounter;

// Debuger
Debugger debugger(encoders, performanceCounter, sensorsCollection, Serial);

Command *commands[COMMAND_COUNT];

// Command processor
CommandProcessor commandProcessor(
    commands,
    Serial
);

void setup() {
    for (int i = WHEEL_LEFT; i < WHEEL_COUNT; i++) {
        encoders[i].init();
    }

    for (int i = 0; i < IR_SENSORS_COUNT; i++) {
        sensors[i].init();
    }

    for (int i = WHEEL_LEFT; i < WHEEL_COUNT; i++) {
        motors[i].init();
    }

    commands[COMMAND_CHECK] = new CommandCheck();
    commands[COMMAND_RESET] = new CommandReset(chassis);
    commands[COMMAND_UNKNOWN] = new CommandUnknown();
    commands[COMMAND_GETIRVAL] = new CommandGetIrVal(sensorsCollection);
    commands[COMMAND_GETENVAL] = new CommandGetEnVal(chassis);
    commands[COMMAND_GETENVEL] = new CommandGetEnVel(chassis);
    commands[COMMAND_STOP] = new CommandStop(chassis);
    commands[COMMAND_SETPWM] = new CommandSetPwm(chassis);
    commands[COMMAND_GETPWM] = new CommandGetPwm(chassis);
    commands[COMMAND_DEBUG_DISABLE] = new CommandDebugDisable(debugger);
    commands[COMMAND_DEBUG_ENABLE] = new CommandDebugEnable(debugger);
    commands[COMMAND_GETPERF] = new CommandGetPerf(performanceCounter);

    Serial.begin(SERIAL_CONNECTION_SPEED);
}

void loop(void) {
    // serial connection
    commandProcessor.readCommand();
    // encoders on chassis
    chassis.updateEncoders();
    // ir sensors
    sensorsCollection.update();
    // stat
    performanceCounter.update();
    // debug
    debugger.update();
}
