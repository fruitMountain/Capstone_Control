#include <Arduino.h>
#include "Controller.h"

Controller::Controller (double Cp,double Ci,double Cd) : Cp(Cp), Ci(Ci), Cd(Cd) {}

double Controller::Proportional (double error){
  return (error * Cp);
};

double Controller::Integral (double error){
  ESum = ESum + (error * (micros() - dt)/1000);
  dt = micros();
  return (ESum * Ci);
};

double Controller::Derivative (double error){
  double Ediff = Emem - error;
  Emem = error;

  return (Ediff * Cd);
};

double Controller::PID (double error){
  int gain = Controller::Proportional(error) + Controller::Integral(error) + Controller::Derivative(error);
  return(gain);
    };

void Controller::changeP (double param) {
  Cp = param;
}

void Controller::changeI (double param) {
  Ci = param;
}

void Controller::changeD (double param) {
  Cd = param;
}
