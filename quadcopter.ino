/*
  Arduino / C++
  Written by: Seth Berryhill
  Email: seth.p.berryhill@gmail.com
 */

#include "src/sensor/sensor.h"
#include "src/motor/motor.h"
#include "src/controller/controller.h"

/*'debug' changes ESC setup behavior, and triggers
  some printing at the end of main loop */
int debug = 1;

unsigned long time;
float dt;

//Defined in controller.h
Controller ctrl(2.0, 0.4, 0.918);

void setup () {
  Serial.begin(9600);

  /*Initializing Devices:
    mpuSetup is defined in sensor.h
    escSetup is defined in motor.h */
  mpuSetup();
  escSetup();
}

void loop() {

  /*Allows for quickly changing gain coefficients
    defined in the controller object.

  Commands are structured as a letter followed by
  a number without a space. No negative numbers or
  decimals allowed but the number is divided by 1000
  before being applied to the controller */
  if (Serial.available() ) {

    char c = Serial.read();

    switch (c) {
    case 'P':
      ctrl.changeP();

    case 'I':
      ctrl.changeI();

    case 'D':
      ctrl.changeD();

    default:
      break;
    }
  }

  //Updates global dt variable
  dt = (millis() - time) / 1000.0;
  time = millis();

  //Updates controller and sends it to the motors
  sample* data = getSample();
  double error = data->pitch / 16.0;
  int pid = ctrl.PID(error);
  turnMotor(pid);

  if (debug == 1) {
    Serial.print(millis() / 1000.);Serial.print("\t");
    Serial.print(error);Serial.print(",");
    Serial.println(pid);
  }
}
