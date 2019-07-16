/*
  Arduino / C++
  Written by: Seth Berryhill
 */

#include "src/sensor/sensor.h"
#include "src/motor/motor.h"
#include "src/controller/controller.h"

//Defined in controller.h
Controller ctrl(1.125, 0, 0);

int debug = 0;

void setup () {
  Serial.begin(9600);

  /*Initializing Devices:
    mpuSetup is defined in sensor.h
    escSetup is defined in motor.h  */
  mpuSetup();
  escSetup();
}


void loop() {

  double test = 0.0001 * map(analogRead(A0),0,1023,0,100);
  ctrl.changeI(test);

  sample* data = getSample();
  double error = complementary(data);
  int pid = ctrl.PID(error);

  turnMotor(pid);


  //Serial.print(millis());Serial.print("\t");
  Serial.print(error);Serial.print("\t");
  Serial.println(test);


  if (debug == 1) {
    printMotor();
    Serial.print(error);Serial.print("\t");
    Serial.println(pid);
  }
}
