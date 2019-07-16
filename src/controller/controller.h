#pragma once
#include <Arduino.h>

class Controller {
 private:
  double Cp, Ci, Cd;
  double ESum, Emem;
  unsigned long dt;

  double Proportional (double error);
  double Integral (double error);
  double Derivative (double error);

 public:
  Controller (double Cp,double Ci,double Cd);
  double PID (double error);

  void changeP (double param);
  void changeI (double param);
  void changeD (double param);
};
