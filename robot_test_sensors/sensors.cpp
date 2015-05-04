/**
 * This program is for testing IR sensors.
 *
 * Pressing the button changes active sensor to dump it's values to serial port.
 */

#include "Arduino.h"

#define LASER_SENSORS_COUNT 5
unsigned int laserSensors[LASER_SENSORS_COUNT] = {A2, A3, A0, A1, A7};
#define ANALOG_READ_REPEAT 3

int currentLaserSensor = -1;

#define LED_PIN 13
#define BTN_PIN 12

long printDelay = 0;
long loopCount = 0;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    pinMode(BTN_PIN, INPUT_PULLUP);

    for (int i = 0; i < LASER_SENSORS_COUNT; i++) {
        pinMode(laserSensors[i], INPUT);
    }

    Serial.begin(9600);
    Serial.println("Starting the sparkfun robot.");
}

void loop() {
    if (digitalRead(BTN_PIN) == LOW) {
        currentLaserSensor = (currentLaserSensor + 1) % (LASER_SENSORS_COUNT + 1);
        delay(500);
    }

    int analogSensorValue = 0;
    for (int i = 0; i < LASER_SENSORS_COUNT; i++) {
        analogSensorValue = 0;
        for (int j = 0; j < ANALOG_READ_REPEAT; j++) {
            analogSensorValue += analogRead(laserSensors[i]);
        }
        analogSensorValue /= ANALOG_READ_REPEAT;

        if (i == currentLaserSensor) {
            Serial.print("Sensor=");
            Serial.print(i);
            Serial.print(" value=");
            Serial.println(analogSensorValue);
        }
    }

    loopCount++;
    if (printDelay + 1000 < millis()) {
        Serial.println(loopCount);

        loopCount = 0;
        printDelay = millis();
    }
}
