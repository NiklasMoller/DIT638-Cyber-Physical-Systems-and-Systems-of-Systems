#include "../AngleCalculator.hpp"
#include "MathHelper.hpp"

#include "catch.hpp"
#include <iostream>

TEST_CASE("Test 1: getAngleBetweenCones") {

  MathHelper mh;
  AngleCalculator ac;

  // Setup

  // Mocking blue mass centre points
  /* _________________
     |      (20, 0)
     |   (10,10)
     |(0,20)
     |
     |
  */
  std::vector<cv::Point2f> blueMocked = {cv::Point2f(0, 20), cv::Point2f(20, 0),
                                         cv::Point2f(10, 10)};

  // Run
  float blueAngle = ac.getAngleBetweenCones(blueMocked);
  float blueTwoDecimals = mh.roundAFloat(blueAngle);
  float assertedAngleInDegreesBlue = 90.00f;

  // assert
  REQUIRE(blueTwoDecimals == assertedAngleInDegreesBlue);
}

TEST_CASE("Test 2: getAngleBetweenCones") {

  MathHelper mh;
  AngleCalculator ac;

  // Setup

  // Mocking yellow mass centre points
  /* ___________________________
     |                 (40, 0)
     |
     |        (25,10)
     |
     |     (20,20)
     |
     |
  */
  std::vector<cv::Point2f> yellowMocked = {
      cv::Point2f(40, 0), cv::Point2f(20, 20), cv::Point2f(25, 10)};

  // Run
  float yellowAngle = ac.getAngleBetweenCones(yellowMocked);
  float yellowTwoDecimals = mh.roundAFloat(yellowAngle);
  float assertedAngleInDegreesYellow = 90.00f;

  // assert
  REQUIRE(yellowTwoDecimals == assertedAngleInDegreesYellow);
}

TEST_CASE("Test 3: getAngleOfRoad") {

  MathHelper mh;
  AngleCalculator ac;

  // Setup

  // Mocking yellow mass centre points
  /* ___________________________
     |                 (40, 0)
     |
     |        (25,10)
     |
     |     (20,20)
     |
     |
  */
  std::vector<cv::Point2f> yellowMocked = {
      cv::Point2f(40, 0), cv::Point2f(20, 20), cv::Point2f(25, 10)};

  std::vector<cv::Point2f> oneMassCentrePoint = {cv::Point2f(40, 0)};

  // Run
  float angle1 = ac.getAngleOfRoad(yellowMocked, oneMassCentrePoint);
  float angle1TwoDecimals = mh.roundAFloat(angle1);
  float angle1Expected = 90.00f;

  // assert
  REQUIRE(angle1TwoDecimals == angle1Expected);
}

TEST_CASE("Test 4: getAngleOfRoad") {

  MathHelper mh;
  AngleCalculator ac;

  // Setup

  // Mocking blue mass centre points
  /* _________________
     |      (20, 0)
     |   (10,10)
     |(0,20)
     |
     |
  */
  std::vector<cv::Point2f> blueMocked = {cv::Point2f(0, 20), cv::Point2f(20, 0),
                                         cv::Point2f(10, 10)};

  std::vector<cv::Point2f> oneMassCentrePoint = {cv::Point2f(40, 0)};

  // Run
  float angle2 = ac.getAngleOfRoad(oneMassCentrePoint, blueMocked);
  float angle2TwoDecimals = mh.roundAFloat(angle2);
  float angle2Expected = 90.00f;

  // assert
  REQUIRE(angle2TwoDecimals == angle2Expected);
}

TEST_CASE("Test 5: getAngleOfRoad") {

  MathHelper mh;
  AngleCalculator ac;

  // Setup

  // Mocking blue mass centre points
  /* _________________
     |      (20, 0)
     |   (10,10)
     |(0,20)
     |
     |
  */
  std::vector<cv::Point2f> blueMocked = {cv::Point2f(0, 20), cv::Point2f(20, 0),
                                         cv::Point2f(10, 10)};

  // Mocking yellow mass centre points
  /* ___________________________
     |                 (40, 0)
     |
     |        (25,10)
     |
     |     (20,20)
     |
     |
  */
  std::vector<cv::Point2f> yellowMocked = {
      cv::Point2f(40, 0), cv::Point2f(20, 20), cv::Point2f(25, 10)};

  // Run
  float angle3 = ac.getAngleOfRoad(yellowMocked, blueMocked);
  float angle3TwoDecimals = mh.roundAFloat(angle3);
  float angle3Expected = 90.00f;

  // assert
  REQUIRE(angle3TwoDecimals == angle3Expected);
}

TEST_CASE("Test 6: getAngleOfRoad") {

  MathHelper mh;
  AngleCalculator ac;

  // Setup
  std::vector<cv::Point2f> oneMassCentrePoint = {cv::Point2f(40, 0)};

  // Run
  float angle4 = ac.getAngleOfRoad(oneMassCentrePoint, oneMassCentrePoint);
  float angle4TwoDecimals = mh.roundAFloat(angle4);
  float angle4Expected = 90.00f;

  // assert
  REQUIRE(angle4TwoDecimals == angle4Expected);
}