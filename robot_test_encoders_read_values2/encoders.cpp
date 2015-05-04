/**
 * This program is for testing encoders connected to analog inputs with filter.
 *
 * It uses filter on raw readings, takes average on every 3 values.
 */

#include <Arduino.h>

#define LEFT_WHEEL_ENCODER_PIN A4
#define RIGHT_WHEEL_ENCODER_PIN A5
#define BTN_PIN 12

void setup() {
    pinMode(BTN_PIN, INPUT_PULLUP);

    pinMode(LEFT_WHEEL_ENCODER_PIN, INPUT);
    pinMode(RIGHT_WHEEL_ENCODER_PIN, INPUT);

    Serial.begin(9600);
    Serial.println("==========================");
}

#define ANALOG_READ_BUFFER_COUNT 100
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
int tmpIndex = 0;

#define LOW_VALUE_THRESHOLD 300
#define HIGH_VALUE_THRESHOLD 3000

long loopCount = 0;
unsigned long printDelay = 0;

void loop(void) {
    loopCount++;

    if (digitalRead(BTN_PIN) == LOW) {
        leftCounter = 0;
        rightCounter = 0;
        delay(1000);
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

    if (printDelay + 1000 < millis()) {
        Serial.print("left=");
        Serial.print(leftCounter);
        Serial.print(" right=");
        Serial.print(rightCounter);
        Serial.print(" loops=");
        Serial.println(loopCount);

        loopCount = 0;
        printDelay = millis();
    }
}
