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

// Encoder objects
Encoder encoders[WHEEL_COUNT] = {Encoder(LEFT_WHEEL_ENCODER_PIN), Encoder(RIGHT_WHEEL_ENCODER_PIN)};
IrSensor sensors[IR_SENSORS_COUNT] = {
    IrSensor(IR_SENSOR_1_PIN),
    IrSensor(IR_SENSOR_2_PIN),
    IrSensor(IR_SENSOR_3_PIN),
    IrSensor(IR_SENSOR_4_PIN),
    IrSensor(IR_SENSOR_5_PIN)
};

int debug = 0;
unsigned long perf = 0;

unsigned long tmpTime = 0;
int tmpValue = 0;
unsigned int tmpIndex = 0;

unsigned long loopCount = 0;
unsigned long printDelay = 0;

unsigned int wheelPins[WHEEL_COUNT][WHEEL_PIN_COUNT] = {{2, 4, 5},{7, 8, 6}};

unsigned int sensorReadedCount = 0;
#define SENSOR_READ_DELAY 1000 / IR_SENSORS_COUNT
unsigned long sensorReadedTime = 0;

String commandBuffer;

int leftWheelPwm = 0;
int rightWheelPwm = 0;

#define PWM_MAX_VALUE 255

void driveWheel(WHEEL wheel, int spd) {
    if (spd < -PWM_MAX_VALUE || spd > PWM_MAX_VALUE) {
        return;
    }

    if (wheel == WHEEL_LEFT) {
        leftWheelPwm = spd;
        encoders[WHEEL_LEFT].setDirection(spd);
    } else {
        rightWheelPwm = spd;
        encoders[WHEEL_RIGHT].setDirection(spd);
    }

    if (spd < 0) {
        digitalWrite(wheelPins[wheel][WHEEL_PIN_EN1], LOW);
        digitalWrite(wheelPins[wheel][WHEEL_PIN_EN2], HIGH);
        analogWrite(wheelPins[wheel][WHEEL_PIN_PWM], -spd);
    } else {
        digitalWrite(wheelPins[wheel][WHEEL_PIN_EN1], HIGH);
        digitalWrite(wheelPins[wheel][WHEEL_PIN_EN2], LOW);
        analogWrite(wheelPins[wheel][WHEEL_PIN_PWM], spd);
    }
}

void stopWheel(WHEEL wheel, bool active) {
    if (wheel == WHEEL_LEFT) {
        leftWheelPwm = 0;
        encoders[WHEEL_LEFT].setDirection(1);
    } else {
        rightWheelPwm = 0;
        encoders[WHEEL_RIGHT].setDirection(1);
    }
    digitalWrite(wheelPins[wheel][WHEEL_PIN_EN1], active? HIGH: LOW);
    digitalWrite(wheelPins[wheel][WHEEL_PIN_EN2], active? HIGH: LOW);
    analogWrite(wheelPins[wheel][WHEEL_PIN_PWM], 0);
}

void setup() {
    for (int i = WHEEL_LEFT; i < WHEEL_COUNT; i++) {
        encoders[i].init();
    }

    for (int i = 0; i < IR_SENSORS_COUNT; i++) {
        sensors[i].init();
    }

    for (int i = 0; i < WHEEL_COUNT; i++) {
        for (int j = 0; j < WHEEL_PIN_COUNT; j++) {
            pinMode(wheelPins[i][j], OUTPUT);
            digitalWrite(wheelPins[i][j], LOW);
        }
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
        stopWheel(WHEEL_LEFT, false);
        stopWheel(WHEEL_RIGHT, false);
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

        driveWheel(WHEEL_LEFT, data[0]);
        driveWheel(WHEEL_RIGHT, data[1]);
        return 0;
    }

    if (input.equals("PWM?")) {
        output.concat("[");
        output.concat(leftWheelPwm);
        output.concat(", ");
        output.concat(rightWheelPwm);
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

void processIrSensors() {
    tmpTime = micros();
    if ((sensorReadedTime + SENSOR_READ_DELAY < tmpTime)
        || (sensorReadedTime > tmpTime)
    ) {
        sensors[sensorReadedCount].update();
        sensorReadedCount = (sensorReadedCount + 1) % IR_SENSORS_COUNT;
        sensorReadedTime = tmpTime;
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
    processIrSensors();
    // stat and debug
    countPerformance();
}
