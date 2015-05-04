/**
 * This program is for dumping raw values from encoder.
 *
 * When pressing the button it starts to read 500 encoder values and prints it into serial connection.
 * It uses average values with every 3 readings.
 */

#include <Arduino.h>

#define LEFT_WHEEL_ENCODER_PIN A5
#define BTN_PIN 12

void setup() {
    pinMode(BTN_PIN, INPUT_PULLUP);

    pinMode(LEFT_WHEEL_ENCODER_PIN, INPUT);

    Serial.begin(9600);
    Serial.println("==========================");
}

#define APROXIMATION_COUNT 3
#define READ_BUFFER_COUNT 500
#define COLS 10
int leftReaded[READ_BUFFER_COUNT];
int leftReadedCount = 0;

int minValue = 2000;
int maxValue = -1;

#define LOW_VALUE_THRESHOLD 100
#define HIGH_VALUE_THRESHOLD 1000

long tmpValue = 0;

void loop(void) {
    if (digitalRead(BTN_PIN) == LOW) {
        for (int i = 0; i < READ_BUFFER_COUNT / COLS; i++) {
            for (int j = 0; j < COLS; j++) {
                int index = i + j * (READ_BUFFER_COUNT / COLS);
                Serial.print("\t");

                tmpValue = 0;
                for (int k = 0; k < APROXIMATION_COUNT; k++) {
                    tmpValue += leftReaded[(READ_BUFFER_COUNT+index-k)%READ_BUFFER_COUNT];
                }
                tmpValue /= APROXIMATION_COUNT;

                if (tmpValue <= LOW_VALUE_THRESHOLD) {
                    Serial.print("<");
                } else {
                    Serial.print(" ");
                }
                if (tmpValue < 10) {
                    Serial.print(" ");
                }
                if (tmpValue < 100) {
                    Serial.print(" ");
                }
                if (tmpValue < 1000) {
                    Serial.print(" ");
                }
                Serial.print(tmpValue);
                if (tmpValue >= HIGH_VALUE_THRESHOLD) {
                    Serial.print(">");
                } else {
                    Serial.print(" ");
                }

                if (tmpValue < minValue) {
                    minValue = (int)tmpValue;
                }
                if (tmpValue > maxValue) {
                    maxValue = (int)tmpValue;
                }
            }
            Serial.println();
        }

        Serial.print("min=");
        Serial.print(minValue);
        Serial.print(" max=");
        Serial.print(maxValue);
        Serial.println();

        minValue = 2000;
        maxValue = -1;

        leftReadedCount = 0;
        Serial.print("3...");
        delay(1000);
        Serial.print("2...");
        delay(1000);
        Serial.print("1...");
        delay(1000);
        Serial.println("Begin");
    }

    if (leftReadedCount < READ_BUFFER_COUNT) {
        leftReaded[leftReadedCount] = analogRead(LEFT_WHEEL_ENCODER_PIN);
        leftReadedCount++;
        _delay_us(300);
    }
}
