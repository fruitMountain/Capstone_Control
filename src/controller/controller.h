#pragma once
#include <Arduino.h>


class Controller {
 private:
  //gain coefficients
  double Cp, Ci, Cd;
  //summed and previous error terms
  double ESum, Emem;

  //control functions
  double Proportional (double error);
  double Integral (double error);
  double Derivative (double error);

 public:
  //constructor
  Controller (double Cp,double Ci,double Cd);

  double PID (double error);

  //functions for changing gain coefficients
  void getInt (double& Int);
  void changeP ();
  void changeI ();
  void changeD ();
};

//time between loop iterations, defined in main
extern float dt;
