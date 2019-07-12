/*
  Arduino / C++
  Written by: Seth Berryhill
 */

#include "src/sensor/sensor.h"
#include "src/motor/motor.h"
#include "src/controller/controller.h"

//Defined in controller.h
Controller ctrl(2.4, 0.15, .3);

int debug = 0;

sample* data;
double error;
int pid;

void setup () {
  Serial.begin(9600);

  /*Initializing Devices:
    mpuSetup is defined in sensor.h
    escSetup is defined in motor.h  */
  mpuSetup();
  escSetup();
}


void loop() {

  data = getSample();
  error = complementary(data);
  pid = ctrl.PID(error);

  turnMotor(pid);


  if (debug == 1) {
    printMotor();
    Serial.print(error);Serial.print("\t");
    Serial.println(pid);
  }
}
