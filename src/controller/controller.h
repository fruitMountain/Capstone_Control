#pragma once
#include <Arduino.h>

class Controller{
 private:
  int Cp, Ci, Cd;
  int ESum, Emem;

  float Proportional(int error);
  float Integral(int error);
  float Derivative(int error);

 public:
  Controller(float Cp,float Ci,float Cd);
  float PID(int error);
};
