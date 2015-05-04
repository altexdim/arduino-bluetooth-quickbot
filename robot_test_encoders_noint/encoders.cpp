/**
 * Program for testing encoders connected to the digital input pins.
 *
 * Every loop it reads values from the encoders.
 * This implementation has issues when wheels are been rotating too fast or too slow.
 */

#include <Arduino.h>

#define LEFT_WHEEL_ENCODER_PIN 10
#define RIGHT_WHEEL_ENCODER_PIN 11

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

#define ANTI_JITTER_DELAY 1000

void setup() {
    pinMode(BTN_PIN, INPUT_PULLUP);

    pinMode(LEFT_WHEEL_ENCODER_PIN, INPUT_PULLUP);
    pinMode(RIGHT_WHEEL_ENCODER_PIN, INPUT_PULLUP);

    Serial.begin(9600);
    Serial.println("left    right   loops");
    Serial.println("==========================");
}

void loop(void) {
    loopCount++;

    if (digitalRead(BTN_PIN) == LOW) {
        lCount = 0;
        rCount = 0;
    }

    if (digitalRead(LEFT_WHEEL_ENCODER_PIN) == LOW) {
        if (leftWheelState && (lastLFall + ANTI_JITTER_DELAY < micros())) {
            lastLRise = micros();
            leftWheelState = 0;
            lCount++;
        }
    } else {
        if (!leftWheelState && (lastLRise + ANTI_JITTER_DELAY < micros())) {
            lastLFall = micros();
            leftWheelState = 1;
        }
    }

    if (digitalRead(RIGHT_WHEEL_ENCODER_PIN) == LOW) {
        if (rightWheelState && (lastRFall + ANTI_JITTER_DELAY < micros())) {
            lastRRise = micros();
            rightWheelState = 0;
            rCount++;
        }
    } else {
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
