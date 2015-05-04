/**
 * Program for testing encoders connected to analog inputs.
 *
 * In the main cycle the values are readed from ADC.
 * Works quite good. Does not use filter.
 */

#include <Arduino.h>

#define LEFT_WHEEL_ENCODER_PIN A4
#define RIGHT_WHEEL_ENCODER_PIN A5

long lCount = 0;
long rCount = 0;

long lastRRise = 0;
long lastLRise = 0;
long lastRFall = 0;
long lastLFall = 0;

#define BTN_PIN 12

long printDelay = 0;
long loopCount = 0;

int leftWheelState = 1;
int rightWheelState = 1;

#define ANTI_JITTER_DELAY 300

void setup() {
    pinMode(BTN_PIN, INPUT_PULLUP);

    pinMode(LEFT_WHEEL_ENCODER_PIN, INPUT);
    pinMode(RIGHT_WHEEL_ENCODER_PIN, INPUT);

    Serial.begin(9600);
    Serial.println("left    right   loops");
    Serial.println("==========================");
}

#define APROXIMATION_COUNT 3
int leftReaded[APROXIMATION_COUNT];
int leftReadedCount = 0;
int rightReaded[APROXIMATION_COUNT];
int rightReadedCount = 0;

#define LOW_TRIGGER_VALUE 500
#define HIGH_TRIGGER_VALUE 600

long getValue(int values[APROXIMATION_COUNT]) {
    long value = 0;
    for (int i = 0; i < APROXIMATION_COUNT; i++) {
        value += values[i];
    }
    return value / APROXIMATION_COUNT;
}

void loop(void) {
    loopCount++;

    if (digitalRead(BTN_PIN) == LOW) {
        lCount = 0;
        rCount = 0;
    }

    leftReaded[(leftReadedCount++)%APROXIMATION_COUNT] = analogRead(LEFT_WHEEL_ENCODER_PIN);
    long value = getValue(leftReaded);
//    Serial.println(value);
    if (value < LOW_TRIGGER_VALUE) {
        if (leftWheelState && (lastLFall + ANTI_JITTER_DELAY < micros())) {
            lastLRise = micros();
            leftWheelState = 0;
            lCount++;
        }
    } else if (value > HIGH_TRIGGER_VALUE) {
        if (!leftWheelState && (lastLRise + ANTI_JITTER_DELAY < micros())) {
            lastLFall = micros();
            leftWheelState = 1;
        }
    }

    rightReaded[(rightReadedCount++)%APROXIMATION_COUNT] = analogRead(LEFT_WHEEL_ENCODER_PIN);
    value = getValue(rightReaded);
    if (value < LOW_TRIGGER_VALUE) {
        if (rightWheelState && (lastRFall + ANTI_JITTER_DELAY < micros())) {
            lastRRise = micros();
            rightWheelState = 0;
            rCount++;
        }
    } else if (value > HIGH_TRIGGER_VALUE) {
        if (!rightWheelState && (lastRRise + ANTI_JITTER_DELAY < micros())) {
            lastRFall = micros();
            rightWheelState = 1;
        }
    }

    if (printDelay + 1000 < millis()) {
        Serial.print(lCount);
        Serial.print("\t");
        Serial.print(rCount);
        Serial.print("\t");
        Serial.println(loopCount);

        loopCount = 0;
        printDelay = millis();
    }
}
