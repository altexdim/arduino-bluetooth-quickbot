/**
 * Connection settings of the peripherals hardware.
 */

#ifndef SETTINGS_H
#define SETTINGS_H

// Pin connected to left encoder
#define LEFT_WHEEL_ENCODER_PIN A5
// Pin connected to right encoder
#define RIGHT_WHEEL_ENCODER_PIN A4

// Number of IR sensors used
#define IR_SENSORS_COUNT 5
// Pins connected to 1-5 ir sensor
#define IR_SENSOR_1_PIN A2
#define IR_SENSOR_2_PIN A3
#define IR_SENSOR_3_PIN A0
#define IR_SENSOR_4_PIN A1
#define IR_SENSOR_5_PIN A7

#define LEFT_MOTOR_EN1_PIN 2
#define LEFT_MOTOR_EN2_PIN 4
#define LEFT_MOTOR_PWM_PIN 5
#define RIGHT_MOTOR_EN1_PIN 7
#define RIGHT_MOTOR_EN2_PIN 8
#define RIGHT_MOTOR_PWM_PIN 6

#endif
