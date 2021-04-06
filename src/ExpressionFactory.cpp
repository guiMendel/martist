#include "../include/ExpressionFactory.hpp"
#include <math.h>

#define PI 3.14159265

double ExpressionFactory::sin(double input) {
  return ::sin(PI * input);
}

double ExpressionFactory::cosin(double input) {
  return cos(PI * input);
}

double ExpressionFactory::product(double a, double b) {
  return a * b;
}

double ExpressionFactory::mean(double a, double b) {
  return (a + b) / 2.0;
}