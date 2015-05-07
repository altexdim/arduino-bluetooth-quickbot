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

int debug = 0;
unsigned long perf = 0;

unsigned long loopCount = 0;
unsigned long printDelay = 0;

String commandBuffer;

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

    Serial.begin(115200);
    Serial.println("===== QuickBot has been initialized =====");
}

void resetEncoders() {
    encoders[WHEEL_LEFT].reset();
    encoders[WHEEL_RIGHT].reset();
}

/**
    // ---------- all commands ----------
    [DONE] msgResult.group('CMD') == 'CHECK':
    [DONE] msgResult.group('CMD') == 'PWM':        msgResult.group('QUERY'):
    [DONE] msgResult.group('CMD') == 'PWM':        msgResult.group('SET') and msgResult.group('ARGS'):
    [DONE] msgResult.group('CMD') == 'IRVAL':      msgResult.group('QUERY'):
    [SKIP] msgResult.group('CMD') == 'ULTRAVAL':   msgResult.group('QUERY'):
    [DONE] msgResult.group('CMD') == 'ENVAL':      msgResult.group('QUERY'):
    [DONE] msgResult.group('CMD') == 'ENVEL':      msgResult.group('QUERY'):
    [DONE] msgResult.group('CMD') == 'RESET':
    [SKIP] msgResult.group('CMD') == 'UPDATE':     msgResult.group('SET') and msgResult.group('ARGS'):
    [SKIP] msgResult.group('CMD') == 'END':

    // ---------- commands from simulator ----------
    [DONE] def send_halt(self):                        connection.sendtorobot('PWM=0,0')
    [DONE] def set_pwm(self, l, r, connection):        connection.sendtorobot('PWM={0},{1}'.format(int(l), int(r)))
    [DONE] def get_pwm(self, connection):              connection.sendtorobot('PWM?')

    [DONE] def get_encoder_ticks(self, connection):    connection.sendtorobot('ENVAL?')
    [DONE] def get_encoder_velocity(self, connection): connection.sendtorobot('ENVEL?')
    [DONE] def get_ir_raw_values(self, connection):    connection.sendtorobot('IRVAL?')

    [DONE] def send_reset(self):                       connection.sendtorobot('RESET')
    [DONE] def ping(self):                             connection.sendtorobot('CHECK')
 */
int execudeCommand(String &input, String &output) {
    output = "";

    if (input.equals("CHECK")) {
        output = "Hello from QuickBot";
        return 1;
    }

    if (input.equals("RESET")) {
        resetEncoders();
        output = "Encoder values reset to [0, 0]";
        return 1;
    }

    if (input.equals("IRVAL?")) {
        output.concat("[");
        for (int i = 0; i < IR_SENSORS_COUNT; i++) {
            if (i) {
                output.concat(", ");
            }
            output.concat(sensors[i].getDistance());
        }
        output.concat("]");
        return 1;
    }

    if (input.equals("ENVEL?")) {
        output.concat("[");
        output.concat(encoders[WHEEL_LEFT].getVelocity());
        output.concat(", ");
        output.concat(encoders[WHEEL_RIGHT].getVelocity());
        output.concat("]");
        return 1;
    }

    if (input.equals("ENVAL?")) {
        output.concat("[");
        output.concat(encoders[WHEEL_LEFT].getCounter());
        output.concat(", ");
        output.concat(encoders[WHEEL_RIGHT].getCounter());
        output.concat("]");
        return 1;
    }

    if (input.equals("PWM=0,0")) {
        motors[WHEEL_LEFT].stop();
        motors[WHEEL_RIGHT].stop();
        return 0;
    }

    if (input.startsWith("PWM=")) {
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

        motors[WHEEL_LEFT].drive(data[0]);
        encoders[WHEEL_LEFT].setDirection(motors[WHEEL_LEFT].getPwm());
        motors[WHEEL_RIGHT].drive(data[1]);
        encoders[WHEEL_RIGHT].setDirection(motors[WHEEL_RIGHT].getPwm());
        return 0;
    }

    if (input.equals("PWM?")) {
        output.concat("[");
        output.concat(motors[WHEEL_LEFT].getPwm());
        output.concat(", ");
        output.concat(motors[WHEEL_RIGHT].getPwm());
        output.concat("]");
        return 1;
    }

    if (input.equals("DEBUG=1")) {
        debug = 1;
        output = "Debug set to 1";
        return 1;
    }

    if (input.equals("DEBUG=0")) {
        debug = 0;
        output = "Debug set to 0";
        return 1;
    }

    if (input.equals("PERF?")) {
        output = "Performance = ";
        output.concat(perf);
        return 1;
    }

    output = "UNKNOWN COMMAND: ";
    output.concat(input);
    return 1;
}

void processSerial() {
    while (Serial.available()) {
        int incomingChar = Serial.read();
        if (incomingChar == -1) {
            continue;
        }

        if (incomingChar == '$') {
            commandBuffer = "";
            continue;
        }

        if (incomingChar == '*') {
            String outputBuffer;
            int result = execudeCommand(commandBuffer, outputBuffer);
            if (result) {
                Serial.println(outputBuffer);
            }
            continue;
        }

        commandBuffer += (char)incomingChar;
    }
}

void printDebugInfo() {
    if (debug & 1) {
        Serial.print("left=");
        Serial.print(encoders[WHEEL_LEFT].getCounter());
        Serial.print(" right=");
        Serial.print(encoders[WHEEL_RIGHT].getCounter());
        Serial.print(" loops=");
        Serial.print(perf);

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
    processSerial();
    // encoders
    encoders[WHEEL_LEFT].update();
    encoders[WHEEL_RIGHT].update();
    // ir sensors
    sensorsCollection.update();
    // stat and debug
    countPerformance();
}
