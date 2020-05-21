#include "../DecisionSender.hpp"
#include "MathHelper.hpp"

#include "catch.hpp"

#include <iostream>

TEST_CASE("Test 1: computeGroundSteeringRequest") {

  MathHelper mh;
  DecisionSender ds;

  // Setup
  float angle1 = 90.00;
  float pedalPos = 0.3;

  // Run
  float groundSteeringRequest1 =
      ds.computeGroundSteeringRequest(angle1, pedalPos);
  float twoDecimals1 = mh.roundAFloat(groundSteeringRequest1);
  float expected1 = 0.00f;

  // Assert
  REQUIRE(twoDecimals1 == expected1);
}

TEST_CASE("Test 2: computeGroundSteeringRequest") {

  MathHelper mh;
  DecisionSender ds;

  // Setup
  float angle2 = 45.00;
  float pedalPos = 0.3;

  // Run
  float groundSteeringRequest2 =
      ds.computeGroundSteeringRequest(angle2, pedalPos);
  float twoDecimals2 = mh.roundAFloat(groundSteeringRequest2);
  float expected2 = -0.14f;

  // Assert
  REQUIRE(twoDecimals2 == expected2);
}

TEST_CASE("Test 3: computeGroundSteeringRequest") {

  MathHelper mh;
  DecisionSender ds;

  // Setup
  float angle3 = 180.00;
  float pedalPos = 0.3;

  // Run
  float groundSteeringRequest3 =
      ds.computeGroundSteeringRequest(angle3, pedalPos);
  float twoDecimals3 = mh.roundAFloat(groundSteeringRequest3);
  float expected3 = 0.3f;

  // Assert
  REQUIRE(twoDecimals3 == expected3);
}