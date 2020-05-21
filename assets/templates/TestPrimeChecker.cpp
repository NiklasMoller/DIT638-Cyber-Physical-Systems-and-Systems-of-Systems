#define CATCH_CONFIG_MAIN

#include "PrimeChecker.hpp"
#include "catch.hpp"

TEST_CASE("Test 1 - Test with prime") {
  PrimeChecker pc;
  REQUIRE(pc.isPrime(5) == true);
}

TEST_CASE("Test 2 - Test with non-prime") {
  PrimeChecker pc;
  REQUIRE(pc.isPrime(4) == false);
}