#pragma once
#include <Arduino.h>

class lowPass {
 private:
  double inHistory;

 public:
  double filter (double in, double fc, double dt);
};

class highPass {

 private:
  double inHistory;
  double outHistory;

 public:
  double filter (double in, double fc, double dt);
};
