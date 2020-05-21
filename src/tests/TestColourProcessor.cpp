#include "../ColourProcessor.hpp"
#include "catch.hpp"

#include <iostream>

// Include the GUI and image processing header files from OpenCV
#include <opencv2/opencv.hpp>

TEST_CASE("Test 1") {
  // setup
  bool isIdentical;
  ColourProcessor colourProcessor;
  cv::Mat img =
      cv::imread("/opt/sources/build/CMakeFiles/steering-decider-runner.dir/"
                 "src/tests/test_image.png",
                 cv::IMREAD_COLOR); // change location when run locally

  if (img.empty()) {
    std::cout << "!!! Failed imread(): image not found" << std::endl;
  }

  // run
  std::pair<cv::Mat, cv::Mat> img2 = colourProcessor.processImage(img);

  int processed_img_rows = img2.first.rows;
  int processed_img_cols = img2.first.cols;

  int original_img_rows = img.rows;
  int original_img_cols = img.cols;

  int row_diffs = original_img_rows - processed_img_rows;
  int col_diffs = original_img_cols - processed_img_cols;

  int diffs = row_diffs - col_diffs;

  if (diffs != 0) {
    isIdentical = false;
  } else {
    isIdentical = true;
  }

  // assert
  REQUIRE(isIdentical == false);
}