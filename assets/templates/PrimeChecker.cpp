#include "PrimeChecker.hpp"
#include <string.h>

bool PrimeChecker::isPrime(uint16_t number) {
  bool retVal{true};

  if (number < 2 || 0 == number % 2) {
    retVal = false;
  } else {
    for (uint16_t i{3}; (i * i) <= number; i += 2) {
      if (0 == number % i) {
        return false;
        break;
      }
    }
  }
  return retVal;
}