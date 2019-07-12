#include "motor.h"

/* The arduino treats the ESC as a servo
   so here we construct two ESCs to control */
Servo esc_0;
Servo esc_1;
int speed0, speed1;

void escSetup () {
  esc_0.attach(9);
  esc_1.attach(10);

  if (debug != 1){
  // Wait for input
  Serial.println("When you're ready press any key.");
  while (!Serial.available());
  Serial.read();
  }

  // Send Stop Command
  esc_0.writeMicroseconds(2000);
  esc_1.writeMicroseconds(2000);

  if (debug != 1) {
  // Wait for input
  Serial.println("After two beeps press any key.");
  while (!Serial.available());
  Serial.read();
  }
  else {
    delay(5000);
  }

  // Send Start Command
  esc_0.writeMicroseconds(1000);
  esc_1.writeMicroseconds(1000);

  if (debug != 1) {
  // Wait for input
  Serial.println("After three beeps press any key.");
  while (!Serial.available());
  Serial.read();
  }
  else {
    delay(5000);
  }
};

// A function to apply a speed to the motors
void turnMotor (double control) {
  if (control >= 0) {
    speed0 = idleSpeed + control;
    speed1 = idleSpeed - control;

    if (speed0 < minSpeed) {speed0 = minSpeed;}
    if (speed1 < minSpeed) {speed1 = minSpeed;}

    if (speed0 > maxSpeed) {speed0 = maxSpeed;}
    if (speed1 > maxSpeed) {speed1 = maxSpeed;}
  }

  if (control < 0) {
    speed0 = idleSpeed + control;
    speed1 = idleSpeed - control;

    if (speed0 < minSpeed) {speed0 = minSpeed;}
    if (speed1 < minSpeed) {speed1 = minSpeed;}

    if (speed0 > maxSpeed) {speed0 = maxSpeed;}
    if (speed1 > maxSpeed) {speed1 = maxSpeed;}
  }

  esc_0.writeMicroseconds(speed0);
  esc_1.writeMicroseconds(speed1);
};

void printMotor () {
  Serial.print(speed0);Serial.print("\t");
  Serial.print(speed1);Serial.print("\t");
};
