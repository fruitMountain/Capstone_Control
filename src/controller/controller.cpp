#include <Arduino.h>
#include "Controller.h"

Controller::Controller(float Cp,float Ci,float Cd) : Cp(Cp), Ci(Ci), Cd(Cd) {}

float Controller::Proportional(int error){
  return (error * Cp);
};

float Controller::Integral(int error){
  int ESum = ESum + error;
  return ESum;
};

float Controller::Derivative(int error){
  int Ediff = error - Emem;
  Emem = error;
  return Ediff;
};

float Controller::PID(int error){
  int gain = Controller::Proportional(error) + Controller::Integral(error) + Controller::Derivative(error);
  return(gain);
    };
