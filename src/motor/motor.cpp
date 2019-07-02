#include "motor.h"

/* The arduino treats the ESC as a servo
   so here we construct two ESCs to control */
Servo esc_0;
Servo esc_1;


void escSetup () {
  esc_0.attach(9);
  esc_1.attach(10);

  // Send Stop Command
  esc_0.writeMicroseconds(2000);
  esc_1.writeMicroseconds(2000);
  delay(5000);

  // Send Start Command
  esc_0.writeMicroseconds(1000);
  esc_1.writeMicroseconds(1000);
  delay(5000);

  // Ramps speed up to idle
  for (int i = 1000; i < minSpeed; i++) {
    esc_0.writeMicroseconds(i);
    esc_1.writeMicroseconds(i);
    delay(100);
  }
};

// A function to apply a speed to the motors
void turnMotor (int speed0, int speed1) {
  esc_0.writeMicroseconds(speed0);
  esc_1.writeMicroseconds(speed1);
};
