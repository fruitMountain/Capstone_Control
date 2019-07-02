/*
  Arduino / C++
  Written by: Seth Berryhill
 */

#include "../src/sensor/sensor.h"
#include "../src/motor/motor.h"
#include "../src/controller/controller.h"

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

}
