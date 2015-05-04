/**
 * This program is for testing encoders connected to analog inputs with filter and motors and additional cpu load.
 *
 * As additional load on cpu it uses reading from 5 IR sensors.
 * It tests if the cpu is fast enough to manage all sensors and encoders.
 */

#include <Arduino.h>

#define LEFT_WHEEL_ENCODER_PIN A4
#define RIGHT_WHEEL_ENCODER_PIN A5
#define BTN_PIN 12

#define ANALOG_READ_BUFFER_COUNT 10
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


unsigned long tmpTime = 0;
int tmpValue = 0;
unsigned int tmpIndex = 0;

#define LOW_VALUE_THRESHOLD 300
#define HIGH_VALUE_THRESHOLD 3000

long loopCount = 0;
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

#define SENSOR_READ_BUFFER_COUNT 10
#define LASER_SENSORS_COUNT 5
int analogSensorValue[LASER_SENSORS_COUNT][SENSOR_READ_BUFFER_COUNT];
unsigned int sensorReadedCount = 0;
#define SENSOR_READ_DELAY 1000
unsigned long sensorReadedTime = 0;
unsigned int laserSensors[LASER_SENSORS_COUNT] = {A2, A3, A0, A1, A7};

void driveWheel(WHEEL wheel, int spd) {
    digitalWrite(wheelPins[wheel][WHEEL_PIN_EN1], HIGH);
    digitalWrite(wheelPins[wheel][WHEEL_PIN_EN2], LOW);
    analogWrite(wheelPins[wheel][WHEEL_PIN_PWM], spd);
}

void stopWheel(WHEEL wheel) {
    digitalWrite(wheelPins[wheel][WHEEL_PIN_EN2], LOW);
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

    Serial.begin(9600);
    Serial.println("==========================");
}

int getSensorValue(int i) {
    tmpIndex = ((SENSOR_READ_BUFFER_COUNT + sensorReadedCount - 1) % SENSOR_READ_BUFFER_COUNT);
    tmpValue = analogSensorValue[i][tmpIndex];
    tmpValue += analogSensorValue[i][(SENSOR_READ_BUFFER_COUNT + tmpIndex - 1) % SENSOR_READ_BUFFER_COUNT];
    tmpValue += analogSensorValue[i][(SENSOR_READ_BUFFER_COUNT + tmpIndex - 2) % SENSOR_READ_BUFFER_COUNT];
    return tmpValue / 3;
}

void loop(void) {
    loopCount++;

    if (digitalRead(BTN_PIN) == LOW) {
        leftCounter = 0;
        rightCounter = 0;
        delay(1000);
        driveWheel(WHEEL_LEFT, 150);
        driveWheel(WHEEL_RIGHT, 150);
        printDelay = millis();
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
                leftCounter++;
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
                rightCounter++;
            }
        }

        rightReadedCount++;
        rightReadedTime = tmpTime;
    }

    tmpTime = micros();
    if ((sensorReadedTime + SENSOR_READ_DELAY < tmpTime)
        || (leftReadedTime > tmpTime)
    ) {
        tmpIndex = (sensorReadedCount % SENSOR_READ_BUFFER_COUNT);
        for (int i = 0; i < LASER_SENSORS_COUNT; i++) {
            analogSensorValue[i][tmpIndex] = analogRead(laserSensors[i]);
        }
        sensorReadedCount++;
        sensorReadedTime = tmpTime;
    }

    if (printDelay + 1000 < millis()) {
        stopWheel(WHEEL_LEFT);
        stopWheel(WHEEL_RIGHT);

        Serial.print("left=");
        Serial.print(leftCounter);
        Serial.print(" right=");
        Serial.print(rightCounter);
        Serial.print(" loops=");
        Serial.print(loopCount);

        for (int i = 0; i < LASER_SENSORS_COUNT; i++) {
            Serial.print("\tS");
            Serial.print(i);
            Serial.print("=");
            Serial.print(getSensorValue(i));
        }
        Serial.println();

        loopCount = 0;
        printDelay = millis();
    }
}
