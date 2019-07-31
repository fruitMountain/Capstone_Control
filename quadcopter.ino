/*
  Arduino / C++
  Written by: Seth Berryhill
 */

#include "src/sensor/sensor.h"
#include "src/motor/motor.h"
#include "src/controller/controller.h"

//Defined in controller.h
Controller ctrl(2.0,0.5,.918);

int debug = 1;

unsigned long time;
float dt;

void setup () {
  Serial.begin(9600);

  /*Initializing Devices:
    mpuSetup is defined in sensor.h
    escSetup is defined in motor.h  */
  mpuSetup();
  escSetup();
}

char c;
void loop() {

  //double test = 0.1 * map(analogRead(A0),0,1023,0,50);
  if (Serial.available()){
    c = Serial.read();
  }


  switch (c) {
  case 'P':
    ctrl.changeP();
    c = 'L';
  case 'I':
    ctrl.changeI();
    c = 'L';c
  case 'I':
    ctrl.changeP();
    c = 'L';
  default:
    break;
  }



  dt = (millis() - time) / 1000.0;
  time = millis();

  sample* data = getSample();
  double error = data->pitch / -16.0;
  //double error1 = complementary(data, 10);
  //double error2 = complementary(data, 100);
  int pid = ctrl.PID(-1*error);

  turnMotor(pid);



  Serial.print(millis() / 1000.);Serial.print("\t");
  //Serial.print(error);Serial.print(",");
  Serial.println(pid);
  //Serial.println((180/3.14) * atan2(data->gyroX , data->gyroZ));


  //Serial.print(atan2(data->acclX, data->acclZ)*(180/3.13));Serial.print(",");
  //Serial.print(error);Serial.print(",");
  //Serial.print(error1);Serial.print(",");
  //  Serial.print(dt);Serial.print(",");
  //Serial.print(data->acclX);Serial.print(",");
  //Serial.print(data->acclZ);Serial.print(",");
  //Serial.println(data->gyroY);

  //Serial.print();Serial.print(",");
  //Serial.println(test.filter(atan2(data->acclX, data->acclZ)*(180/3.13),1,dt));
  //Serial.print(millis() / 1000.0);Serial.print(",");
  //Serial.println(error);
  //delay(100);

  if (debug == 1) {
    //printMotor();
    //Serial.print(error);Serial.print(",");
    //Serial.println(pid);
  }
}
