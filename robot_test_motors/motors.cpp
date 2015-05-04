/**
 * This program is for testing the motors.
 *
 * Press the button to start motors. Every pressing changes motors velocity.
 */

#include <Arduino.h>

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

#define BTN_PIN 12
#define DEBUG 0

int testSpd[] = {50, 100, 150, 200, 255};
int currentSpd = -1;

void setup() {
    pinMode(BTN_PIN, INPUT_PULLUP);

    for (int i = 0; i < WHEEL_COUNT; i++) {
        for (int j = 0; j < WHEEL_PIN_COUNT; j++) {
            pinMode(wheelPins[i][j], OUTPUT);
            digitalWrite(wheelPins[i][j], LOW);
        }
    }

    #if DEBUG
    Serial.begin(9600);
    Serial.println("================");
    #endif
}

void driveWheel(WHEEL wheel, int spd) {
    #if DEBUG
    Serial.print("[Drive] wheel=");
    Serial.print(wheel);
    Serial.print(" spd=");
    Serial.print(spd);
    Serial.print(" pin_en1=");
    Serial.print(wheelPins[wheel][WHEEL_PIN_EN1]);
    Serial.print(" pin_en2=");
    Serial.print(wheelPins[wheel][WHEEL_PIN_EN2]);
    Serial.print(" pin_pwm=");
    Serial.println(wheelPins[wheel][WHEEL_PIN_PWM]);
    #endif

    digitalWrite(wheelPins[wheel][WHEEL_PIN_EN1], HIGH);
    digitalWrite(wheelPins[wheel][WHEEL_PIN_EN2], LOW);
    analogWrite(wheelPins[wheel][WHEEL_PIN_PWM], spd);
}

void stopWheel(WHEEL wheel) {
    #if DEBUG
    Serial.print("[Stop] wheel=");
    Serial.print(wheel);
    Serial.print(" pin_en1=");
    Serial.print(wheelPins[wheel][WHEEL_PIN_EN1]);
    Serial.print(" pin_en2=");
    Serial.print(wheelPins[wheel][WHEEL_PIN_EN2]);
    Serial.print(" pin_pwm=");
    Serial.println(wheelPins[wheel][WHEEL_PIN_PWM]);
    #endif

    digitalWrite(wheelPins[wheel][WHEEL_PIN_EN1], LOW);
    digitalWrite(wheelPins[wheel][WHEEL_PIN_EN2], LOW);
    analogWrite(wheelPins[wheel][WHEEL_PIN_PWM], 0);
}

void loop(void) {
    if (digitalRead(BTN_PIN) == LOW) {
        delay(1000);
        currentSpd = (currentSpd + 1) % 5;
        driveWheel(WHEEL_LEFT, testSpd[currentSpd]);
        driveWheel(WHEEL_RIGHT, testSpd[currentSpd]);
        delay(1000);
        stopWheel(WHEEL_LEFT);
        stopWheel(WHEEL_RIGHT);
    }
}
