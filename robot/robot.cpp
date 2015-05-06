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

int debug = 0;
unsigned long perf = 0;

#define LEFT_WHEEL_ENCODER_PIN A4
#define RIGHT_WHEEL_ENCODER_PIN A5
#define BTN_PIN 12

#define ANALOG_READ_BUFFER_COUNT 5
#define ANALOG_READ_DELAY 300

int leftReaded[ANALOG_READ_BUFFER_COUNT];
unsigned int leftReadedCount = 0;
unsigned long leftReadedTime = 0;
long leftCounter = 0;
int leftEncoderState = 1;

int rightReaded[ANALOG_READ_BUFFER_COUNT];
unsigned int rightReadedCount = 0;
unsigned long rightReadedTime = 0;
long rightCounter = 0;
int rightEncoderState = 1;

#define WHEEL_VELOCITY_TIME 1000
#define WHEEL_VELOCITY_SCALER 1000
long leftWheelVelocity = 0;
long rightWheelVelocity = 0;
long leftWheelCounterLast = 0;
long rightWheelCounterLast = 0;
unsigned long wheelVelocityTime = 0;
long leftWheelDirection = 1;
long rightWheelDirection = 1;

unsigned long tmpTime = 0;
int tmpValue = 0;
unsigned int tmpIndex = 0;

#define LOW_VALUE_THRESHOLD 100 * 3
#define HIGH_VALUE_THRESHOLD 1000 * 3

unsigned long loopCount = 0;
unsigned long printDelay = 0;

enum WHEEL {
    WHEEL_LEFT,
    WHEEL_RIGHT,
    WHEEL_COUNT
};

enum WHEEL_PINS {
    WHEEL_PIN_EN1,
    WHEEL_PIN_EN2,
    WHEEL_PIN_PWM,
    WHEEL_PIN_COUNT
};

unsigned int wheelPins[WHEEL_COUNT][WHEEL_PIN_COUNT] = {{2, 4, 5},{7, 8, 6}};

#define SENSOR_READ_BUFFER_COUNT 5
#define LASER_SENSORS_COUNT 5
int analogSensorValue[LASER_SENSORS_COUNT][SENSOR_READ_BUFFER_COUNT];
int analogSensorValueAvg[LASER_SENSORS_COUNT];
unsigned int sensorReadedCount = 0;
#define SENSOR_READ_DELAY 1000 / LASER_SENSORS_COUNT
unsigned long sensorReadedTime = 0;
unsigned int laserSensors[LASER_SENSORS_COUNT] = {A2, A3, A0, A1, A7};
unsigned int laserIndex = 0;

bool drive = false;
bool btnPressed = false;

int incomingChar;

String commandBuffer;
String outputBuffer;

int leftWheelPwm = 0;
int rightWheelPwm = 0;

#define PWM_MAX_VALUE 255

void driveWheel(WHEEL wheel, int spd) {
    if (spd < -PWM_MAX_VALUE || spd > PWM_MAX_VALUE) {
        return;
    }

    if (wheel == WHEEL_LEFT) {
        leftWheelPwm = spd;
        leftWheelDirection = spd < 0? -1: 1;
    } else {
        rightWheelPwm = spd;
        rightWheelDirection = spd < 0? -1: 1;
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
        leftWheelDirection = 1;
    } else {
        rightWheelPwm = 0;
        rightWheelDirection = 1;
    }
    digitalWrite(wheelPins[wheel][WHEEL_PIN_EN1], active? HIGH: LOW);
    digitalWrite(wheelPins[wheel][WHEEL_PIN_EN2], active? HIGH: LOW);
    analogWrite(wheelPins[wheel][WHEEL_PIN_PWM], 0);
}

void setup() {
    pinMode(BTN_PIN, INPUT_PULLUP);

    pinMode(LEFT_WHEEL_ENCODER_PIN, INPUT);
    pinMode(RIGHT_WHEEL_ENCODER_PIN, INPUT);

    for (int i = 0; i < WHEEL_COUNT; i++) {
        for (int j = 0; j < WHEEL_PIN_COUNT; j++) {
            pinMode(wheelPins[i][j], OUTPUT);
            digitalWrite(wheelPins[i][j], LOW);
        }
    }

    for (int i = 0; i < LASER_SENSORS_COUNT; i++) {
        pinMode(laserSensors[i], INPUT);
    }

    Serial.begin(115200);
    Serial.println("===== QuickBot has been initialized =====");
}

void resetEncoders() {
    leftCounter = 0;
    rightCounter = 0;
    leftWheelCounterLast = 0;
    rightWheelCounterLast = 0;
    leftWheelVelocity = 0;
    rightWheelVelocity = 0;
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
        for (int i = 0; i < LASER_SENSORS_COUNT; i++) {
            if (i) {
                output.concat(", ");
            }
            output.concat(analogSensorValueAvg[i]);
        }
        output.concat("]");
        return 1;
    }

    if (input.equals("ENVEL?")) {
        output.concat("[");
        output.concat(leftWheelVelocity);
        output.concat(", ");
        output.concat(rightWheelVelocity);
        output.concat("]");
        return 1;
    }

    if (input.equals("ENVAL?")) {
        output.concat("[");
        output.concat(leftCounter);
        output.concat(", ");
        output.concat(rightCounter);
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

void loop(void) {
    loopCount++;

    if (digitalRead(BTN_PIN) == LOW) {
        btnPressed = true;
        resetEncoders();
        delay(1000);

        driveWheel(WHEEL_LEFT, 255);
        driveWheel(WHEEL_RIGHT, 255);
        printDelay = millis();
        drive = true;
    }

    while (Serial.available()) {
        incomingChar = Serial.read();
        if (incomingChar == -1) {
            continue;
        }

        if (incomingChar == '$') {
            commandBuffer = "";
        } else if (incomingChar == '*') {
            int result = execudeCommand(commandBuffer, outputBuffer);
            if (result) {
                Serial.println(outputBuffer);
            }
        } else {
            commandBuffer += (char)incomingChar;
        }
    }

    tmpTime = micros();
    if ((leftReadedTime + ANALOG_READ_DELAY < tmpTime)
        || (leftReadedTime > tmpTime)
    ) {
        tmpValue = analogRead(LEFT_WHEEL_ENCODER_PIN);
        tmpIndex = leftReadedCount % ANALOG_READ_BUFFER_COUNT;
        leftReaded[tmpIndex] = tmpValue;

        tmpValue += leftReaded[(ANALOG_READ_BUFFER_COUNT + tmpIndex - 1) % ANALOG_READ_BUFFER_COUNT];
        tmpValue += leftReaded[(ANALOG_READ_BUFFER_COUNT + tmpIndex - 2) % ANALOG_READ_BUFFER_COUNT];

        if (tmpValue <= LOW_VALUE_THRESHOLD) {
            if (leftEncoderState) {
                leftEncoderState = 0;
            }
        } else if (tmpValue >= HIGH_VALUE_THRESHOLD) {
            if (!leftEncoderState) {
                leftEncoderState = 1;
                leftCounter+=leftWheelDirection;
            }
        }

        leftReadedCount++;
        leftReadedTime = tmpTime;
    }

    tmpTime = micros();
    if ((rightReadedTime + ANALOG_READ_DELAY < tmpTime)
        || (rightReadedTime > tmpTime)
    ) {
        tmpValue = analogRead(RIGHT_WHEEL_ENCODER_PIN);
        tmpIndex = rightReadedCount % ANALOG_READ_BUFFER_COUNT;
        rightReaded[tmpIndex] = tmpValue;

        tmpValue += rightReaded[(ANALOG_READ_BUFFER_COUNT + tmpIndex - 1) % ANALOG_READ_BUFFER_COUNT];
        tmpValue += rightReaded[(ANALOG_READ_BUFFER_COUNT + tmpIndex - 2) % ANALOG_READ_BUFFER_COUNT];

        if (tmpValue <= LOW_VALUE_THRESHOLD) {
            if (rightEncoderState) {
                rightEncoderState = 0;
            }
        } else if (tmpValue >= HIGH_VALUE_THRESHOLD) {
            if (!rightEncoderState) {
                rightEncoderState = 1;
                rightCounter+=rightWheelDirection;
            }
        }

        rightReadedCount++;
        rightReadedTime = tmpTime;
    }

    tmpTime = micros();
    if ((sensorReadedTime + SENSOR_READ_DELAY < tmpTime)
        || (leftReadedTime > tmpTime)
    ) {
        laserIndex = sensorReadedCount % LASER_SENSORS_COUNT;
        tmpIndex = (sensorReadedCount / LASER_SENSORS_COUNT) % SENSOR_READ_BUFFER_COUNT;

        tmpValue = analogRead(laserSensors[laserIndex]);
        analogSensorValue[laserIndex][tmpIndex] = tmpValue;

        tmpValue += analogSensorValue[laserIndex][(SENSOR_READ_BUFFER_COUNT + tmpIndex - 1) % SENSOR_READ_BUFFER_COUNT];
        tmpValue += analogSensorValue[laserIndex][(SENSOR_READ_BUFFER_COUNT + tmpIndex - 2) % SENSOR_READ_BUFFER_COUNT];
        analogSensorValueAvg[laserIndex] = tmpValue / 3;

        sensorReadedCount++;
        sensorReadedTime = tmpTime;
    }

    if (wheelVelocityTime + WHEEL_VELOCITY_TIME < millis()) {
        leftWheelVelocity = (leftCounter - leftWheelCounterLast) * WHEEL_VELOCITY_SCALER / WHEEL_VELOCITY_TIME;
        leftWheelCounterLast = leftCounter;
        rightWheelVelocity = (rightCounter - rightWheelCounterLast) * WHEEL_VELOCITY_SCALER / WHEEL_VELOCITY_TIME;
        rightWheelCounterLast = rightCounter;
        wheelVelocityTime = millis();
    }

    if (printDelay + 1000 < millis()) {
        perf = loopCount;

        if (btnPressed) {
            if (drive) {
                stopWheel(WHEEL_LEFT, true);
                stopWheel(WHEEL_RIGHT, true);
                drive = false;
            } else {
                stopWheel(WHEEL_LEFT, false);
                stopWheel(WHEEL_RIGHT, false);
                btnPressed = false;
            }
        }

        if (debug & 1) {
            Serial.print("left=");
            Serial.print(leftCounter);
            Serial.print(" right=");
            Serial.print(rightCounter);
            Serial.print(" loops=");
            Serial.print(perf);

            Serial.print(" l_vel=");
            Serial.print(leftWheelVelocity);
            Serial.print(" r_vel=");
            Serial.print(rightWheelVelocity);

            for (int i = 0; i < LASER_SENSORS_COUNT; i++) {
                Serial.print("\tS");
                Serial.print(i);
                Serial.print("=");
                Serial.print(analogSensorValueAvg[i]);
            }
            Serial.println();
        }

        loopCount = 0;
        printDelay = millis();
    }
}
