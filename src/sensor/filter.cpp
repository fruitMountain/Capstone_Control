#include "filter.h"

double lowPass::filter (double in, double fc, double dt) {

  double alpha = ((2*3.14) * fc * dt) / ((2*3.14) * fc * dt + 1);
  double out = (alpha * in)  +  (1-alpha) * inHistory;

  inHistory = out;

  return out;
};

double highPass::filter (double in, double fc, double dt) {

  double alpha = 1 / ((2*3.14) * fc * dt + 1);

  double out = (alpha * outHistory)  +  (alpha) * (in-inHistory);

  inHistory = in;
  outHistory = out;

  return out;
};
