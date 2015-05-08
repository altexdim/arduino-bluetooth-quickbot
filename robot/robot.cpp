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
 */

#include <Arduino.h>
#include "Encoder.h"
#include "Types.h"
#include "Settings.h"
#include "IrSensor.h"
#include "IrSensorsCollection.h"
#include "Motor.h"
#include "Chassis.h"
#include "CommandProcessor.h"
#include "Commands/Command.h"
#include "Commands/Check.h"
#include "Commands/Check.cpp"

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

// Debug mode on/off switch
int debug = 0;

// Performance counter. Counts program loops per second.
unsigned long perf = 0;

Command *commands[COMMAND_COUNT];

// Command processor
CommandProcessor commandProcessor(
    commands,
    Serial,
    chassis,
    sensorsCollection,
    debug,
    perf
);

unsigned long loopCount = 0;
unsigned long printDelay = 0;

void setup() {
    for (int i = WHEEL_LEFT; i < WHEEL_COUNT; i++) {
        encoders[i].init();
    }

    for (int i = 0; i < IR_SENSORS_COUNT; i++) {
        sensors[i].init();
    }

    for (int i = 0; i < WHEEL_COUNT; i++) {
        motors[i].init();
    }

    commands[COMMAND_CHECK] = new CheckCommand();

    Serial.begin(SERIAL_CONNECTION_SPEED);
}

void printDebugInfo() {
    if (debug & 1) {
        Serial.print("left=");
        Serial.print(encoders[WHEEL_LEFT].getCounter());
        Serial.print(" right=");
        Serial.print(encoders[WHEEL_RIGHT].getCounter());
        Serial.print(" loops=");
        Serial.print(perf);

        long test = encoders[WHEEL_LEFT].getVelocity();
        if (test < 0) {
            Serial.print("MINUS");
        } else {
            // FIXME: negative velocity prints big positive values
            Serial.print("PLUS");
        }

        Serial.print(" l_vel=");
        Serial.print(encoders[WHEEL_LEFT].getVelocity());
        Serial.print(" r_vel=");
        Serial.print(encoders[WHEEL_RIGHT].getVelocity());

        for (int i = 0; i < IR_SENSORS_COUNT; i++) {
            Serial.print("\tS");
            Serial.print(i);
            Serial.print("=");
            Serial.print(sensors[i].getDistance());
        }
        Serial.println();
    }
}

void countPerformance() {
    loopCount++;

    if (printDelay + 1000 < millis()) {
        perf = loopCount;
        loopCount = 0;
        printDelay = millis();

        printDebugInfo();
    }
}

void loop(void) {
    // serial connection
    commandProcessor.readCommand();
    // encoders
    encoders[WHEEL_LEFT].update();
    encoders[WHEEL_RIGHT].update();
    // ir sensors
    sensorsCollection.update();
    // stat and debug
    countPerformance();
}
