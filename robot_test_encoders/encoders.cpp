/**
 * 
 * This program tests encoders with interruptions.
 * 
 * Interruptions are used to count pusles from encoders.
 * There are some issues with that implementation.
 * Sometimes pulses can not be counted with acceptable accuracy.
 * For example when you drive too slow or too fast.
 */

#include <Arduino.h>

#define LEFT_WHEEL_ENCODER_PIN 10
#define RIGHT_WHEEL_ENCODER_PIN 11

volatile long lCount = 0;
volatile long rCount = 0;

volatile byte PBMask = 0;
volatile byte lastPC0PinState = 0xF;
volatile unsigned long lastRRise = 0;
volatile unsigned long lastLRise = 0;
#define ENC_HIGH_DELAY 1000

#define PIN_10_MASK 0x04
#define PIN_11_MASK 0x08
#define PORT_B_MASK 0x01

#define BTN_PIN 12

void handlePin10Change(byte PBTemp) {
    // Was it pin 10, AKA PB2?
    if ((lastPC0PinState & PIN_10_MASK) && !(PBTemp & PIN_10_MASK)) {
        // a falling edge
        if (lastLRise + ENC_HIGH_DELAY < micros()) {
            lCount++;
        }
    } else if (!(lastPC0PinState & PIN_10_MASK) && (PBTemp & PIN_10_MASK)) {
        // a rising edge
        lastLRise = micros();
    }
}

void handlePin11Change(byte PBTemp) {
    // Was it pin 11, AKA PB3?
    if ((lastPC0PinState & PIN_11_MASK) && !(PBTemp & PIN_11_MASK)) {
        // a falling edge
        if (lastRRise + ENC_HIGH_DELAY < micros()) {
            rCount++;
        }
    } else if (!(lastPC0PinState & PIN_11_MASK) && (PBTemp & PIN_11_MASK)) {
        // a rising edge
        lastRRise = micros();
    }
}

ISR(PCINT0_vect) {
    byte PBTemp = PINB & PBMask;  // Capture the state of the pins-of-interest now,

    handlePin10Change(PBTemp);
    handlePin11Change(PBTemp);

    lastPC0PinState = PBTemp;
}

void setup() {
    pinMode(BTN_PIN, INPUT_PULLUP);

    pinMode(LEFT_WHEEL_ENCODER_PIN, INPUT_PULLUP);
    pinMode(RIGHT_WHEEL_ENCODER_PIN, INPUT_PULLUP);

    PCMSK0 |= PIN_10_MASK;  // Enable the pin change interrupt for this pin.
    PCMSK0 |= PIN_11_MASK;  // Enable the pin change interrupt for this pin.
    PBMask |= PIN_10_MASK;  // Add this pin to our mask bits for Port B.
    PBMask |= PIN_11_MASK;  // Add this pin to our mask bits for Port B.
    lastPC0PinState = PBMask;
    PCICR |= PORT_B_MASK;   // Enable pin change interrupts for this group.

    Serial.begin(9600);
    Serial.println("left    right");
    Serial.println("================");
}

void loop(void) {
    if (digitalRead(BTN_PIN) == LOW) {
        lCount = 0;
        rCount = 0;
    }

    Serial.print(lCount);
    Serial.print("\t");
    Serial.println(rCount);
}
