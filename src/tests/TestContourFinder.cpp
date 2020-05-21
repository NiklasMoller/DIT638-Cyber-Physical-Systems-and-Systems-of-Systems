#include "../ContourFinder.hpp"
#include "catch.hpp"

#include <iostream>

// Include the GUI and image processing header files from OpenCV
#include <opencv2/opencv.hpp>

TEST_CASE("Test 2") {
  // setup
  bool outcome;
  cv::Mat img =
      cv::imread("/opt/sources/build/CMakeFiles/steering-decider-runner.dir/"
                 "src/tests/test_image.png",
                 cv::IMREAD_GRAYSCALE); // change location when run locally

  if (img.empty()) {
    std::cout << "!!! Failed imread(): image not found" << std::endl;
  }

  std::pair<cv::Mat, cv::Mat> image_pair;
  image_pair.first = img;
  image_pair.second = img;

  // run
  ContourFinder contourFinder;
  std::pair<std::vector<cv::Point2f>, std::vector<cv::Point2f>> pair;

  try {
    pair = contourFinder.findContours(image_pair, img);
  } catch (const std::exception &e) {
    std::cout << e.what(); // information from length_error printed
  }

  if (pair.first.front().y > 0) {
    outcome = false;
  } else {
    outcome = true;
  }

  // assert
  REQUIRE(outcome == false);
}