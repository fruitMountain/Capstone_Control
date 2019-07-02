#pragma once

#include <Arduino.h>
#include <Servo.h>
// The arduino treats ESCs as Servos

// The speed at idle
#define minSpeed 1175

// Constructs two ESCs in motor.cpp
extern Servo esc_0;
extern Servo esc_1;

/* Setup ESCs

 *  To Calibrate ESC:
 *  1. Disconnect Power supply
 *  2. Set pwm to max signal (2000 us)
 *  3. Reconnect Power Supply, then wait a couple seconds
 *  4. Set pwm to min signal (1000 us)
 *  5. Wait a couple of seconds, then set signal as desired
 *
 *  With this code:
 *  1. Disconnect the power
 *  2. Hit reset on Arduino
 *  3. Reconnect the power
 */

// Function definitions in motor.cpp or README
void escSetup ();
void turnMotor (int speed0, int speed1);
