/*
  Arduino / C++
  Written by: Seth Berryhill
 */

#include "src/sensor/sensor.h"
#include "src/motor/motor.h"
#include "src/controller/controller.h"

//Defined in controller.h
Controller ctrl(2.4, 0.15, .3);

void setup () {
  Serial.begin(9600);

  /*Initializing Devices:
    mpuSetup is defined in sensor.h
    escSetup is defined in motor.h  */
  mpuSetup();
  escSetup();
}


void loop() {

  sample* data = getSample();
  double angle = atan2(data->acclZ , data->acclX) * (180/3.14);
  error = 90 - angle;
  double pid = ctrl.PID(error);

  turnMotor(pid);

  if (debug == 1) {
    printMotor();
    Serial.println(error);Serial.print("\t");
    Serial.println(pid);
  }
}
