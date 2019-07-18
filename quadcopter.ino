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

unsigned long time;
float dt;

void setup () {
  Serial.begin(9600);

  /*Initializing Devices:
    mpuSetup is defined in sensor.h
    escSetup is defined in motor.h  */
  mpuSetup();
  //escSetup();
}

double aHist;
double gHist;
double fHist;


void loop() {
  /*
  double test = 0.0001 * map(analogRead(A0),0,1023,0,100);
  ctrl.changeI(test);
  */

  dt = (millis() - time) / 1000.0;
  time = millis();

  sample* data = getSample();
  double error = complementary(data);
  int pid = ctrl.PID(error);

  //turnMotor(pid);

  float test1 = data->acclX * 8.0 / 32767;
  float test2 = data->acclZ * 8.0 / 32767;


  //Serial.print(millis());Serial.print("\t");
  //Serial.print((data->acclZ),HEX);Serial.print(",");
  //Serial.println(error);
  //Serial.println((180/3.14) * atan2(data->gyroX , data->gyroZ));

  float fc = 4;
  double alph = 1 / (2*3.14*fc*dt +1);
  double g = data->gyroY * (-500.0 / 32767);

  double filt = alph*fHist + (1-alph)*(g - gHist);
  gHist = g;
  fHist = filt;

  alph = alph * 2*3.14*fc*dt;
  double acc = 90.0 - (atan2(data->acclZ , data->acclX) * (180.0/3.14));;
  double afilt = alph*acc + (1-alph)*aHist;
  aHist = afilt;


  //Serial.print(acc);Serial.print(",");
  Serial.println(afilt+filt*dt);

  if (debug == 1) {
    printMotor();
    Serial.print(error);Serial.print(",");
    Serial.println(pid);
  }
}
