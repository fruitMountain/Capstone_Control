/*
  Arduino / C++
  Written by: Seth Berryhill
 */

#include "src/sensor/sensor.h"
#include "src/motor/motor.h"
#include "src/controller/controller.h"

//Defined in controller.h
Controller ctrl(1,1,0);

void setup () {
  Serial.begin(9600);

  /*Initializing Devices:
    mpuSetup is defined in sensor.h
    escSetup is defined in motor.h  */
  mpuSetup();
  escSetup();
}


void loop() {
  sample* test = getSample();
  double angle = atan2(test->acclZ , test->acclX) * (180/3.14);
  double error = 90 - angle;
  double pid = ctrl.PID(error);

  if (pid >= 1500) {pid = 0;};

  if (pid >= 0) {
      int speed0 = 1000 + pid;
      int speed1 = minSpeed;

      if (speed1 < minSpeed) {
        speed1 = minSpeed;
    }
      if (speed0 < minSpeed) {
        speed0 = minSpeed;
      }
      turnMotor(speed0, speed1);
  }
  if (pid < 0) {
      int speed1 = 1000 + (-1 * pid);
      int speed0 = minSpeed;

      if (speed0 < minSpeed) {speed0 = minSpeed;}
      if (speed1 < minSpeed) {speed1 = minSpeed;}

      turnMotor(speed0, speed1);
    }







  //Serial.print(test->acclX);Serial.print("\t");
  //Serial.print(test->acclY);Serial.print("\t");
  //Serial.print(test->acclZ);Serial.print("\t");
  Serial.print(error);Serial.print("\t");
  Serial.println(pid);
}
