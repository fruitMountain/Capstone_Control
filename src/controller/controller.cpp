#include <Arduino.h>
#include "Controller.h"

Controller::Controller (double Cp,double Ci,double Cd) : Cp(Cp), Ci(Ci), Cd(Cd) {}

double Controller::Proportional (double error){
  return (error * Cp);
};

double Controller::Integral (double error){
  ESum = ESum + (error * dt);
  return (ESum * Ci);
};

double Controller::Derivative (double error){
  double Ediff = Emem - error;
  Emem = error;

  return (Ediff * Cd);
};

double Controller::PID (double error){
  //Serial.println(Cp);
  int gain = Controller::Proportional(error) + Controller::Integral(error) + Controller::Derivative(error);
  return(gain);
    };

void Controller::getInt (double& Int) {
  if (Serial.available() >= 1) {
    int i = 0;
    int t = 0;
    while (t == 0) {
      char c = Serial.read();
      if (isDigit(c)) {
        i  =  (10 * i) + (c - '0');
      }
      else {
        t = 1;
      }
    }
    Int = i/1000.0;
  }
};

void Controller::changeP () {
  getInt(Cp);
};

void Controller::changeI () {
  getInt(Ci);
};

void Controller::changeD () {
  getInt(Cd);
};
