#include "MathHelper.hpp"

// To round a float into two decimal numbers
// Reference:
// https://www.geeksforgeeks.org/rounding-floating-point-number-two-decimal-places-c-c/
float MathHelper::roundAFloat(float var) {

  float value = (int)(var * 100 + .5);
  return (float)value / 100;
}
