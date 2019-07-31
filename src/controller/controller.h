#pragma once
#include <Arduino.h>


class Controller {
 private:
  double Cp, Ci, Cd;
  double ESum, Emem;

  double Proportional (double error);
  double Integral (double error);
  double Derivative (double error);

 public:
  Controller (double Cp,double Ci,double Cd);
  double PID (double error);


  void getInt (double& Int);
  void changeP ();
  void changeI ();
  void changeD ();
};

extern float dt;
