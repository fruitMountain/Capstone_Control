#include <Arduino.h>
#include "Controller.h"

Controller::Controller(double Cp,double Ci,double Cd) : Cp(Cp), Ci(Ci), Cd(Cd) {}

double Controller::Proportional(double error){
  return (error * Cp);
};

double Controller::Integral(double error){
  ESum = ESum + error;
  return ESum;
};

double Controller::Derivative(double error){
  double Ediff = error - Emem;
  Emem = error;
  return Ediff;
};

double Controller::PID(double error){
  int gain = Controller::Proportional(error) + Controller::Integral(error) + Controller::Derivative(error);
  return(gain);
    };
