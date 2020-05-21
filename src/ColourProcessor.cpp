#include "ColourProcessor.hpp"

// Include the single-file, header-only middleware libcluon to create
// high-performance microservices
#include "cluon-complete-v0.0.127.hpp"

// Include the GUI and image processing header files from OpenCV
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

std::pair<cv::Mat, cv::Mat> ColourProcessor::processImage(cv::Mat image) {

  int iLowHYellow = 20;
  int iHighHYellow = 45;

  int iLowSYellow = 100;
  int iHighSYellow = 235;

  int iLowVYellow = 124;
  int iHighVYellow = 255;

  int iLowHBlue = 109;
  int iHighHBlue = 135;

  int iLowSBlue = 68;
  int iHighSBlue = 250;

  int iLowVBlue = 52;
  int iHighVBlue = 255;

  int iReflectionLowHBlue = 0;
  int iReflectionHighHBlue = 179;

  int iReflectionLowSBlue = 50;
  int iReflectionHighSBlue = 80;

  int iReflectionLowVBlue = 0;
  int iReflectionHighVBlue = 255;

  cv::Mat yellowImgInRange;
  cv::Mat blueImgInRange;
  cv::Mat yellowImg;
  cv::Mat blueImg;
  cv::Mat blueReflectionImg;
  cv::Mat blueImgInRangeReflection;
  cv::Mat blueImgAnd;
  cv::Mat finalBlueImg;

  // Crop out top half of image
  cv::Mat croppedImage =
      image(cv::Rect(0, image.rows / 2, image.cols, image.rows / 2));

  cv::cvtColor(croppedImage, yellowImg, CV_BGR2HSV);
  blueImg = yellowImg.clone();
  blueReflectionImg = yellowImg.clone();

  // Colour process to detect yellow cones
  cv::inRange(yellowImg, cv::Scalar(iLowHYellow, iLowSYellow, iLowVYellow),
              cv::Scalar(iHighHYellow, iHighSYellow, iHighVYellow),
              yellowImgInRange);

  // Colour process to detect blue cones
  cv::inRange(blueImg, cv::Scalar(iLowHBlue, iLowSBlue, iLowVBlue),
              cv::Scalar(iHighHBlue, iHighSBlue, iHighVBlue), blueImgInRange);

  // Colour process to detect large reflections
  cv::inRange(
      blueReflectionImg,
      cv::Scalar(iReflectionLowHBlue, iReflectionLowSBlue, iReflectionLowVBlue),
      cv::Scalar(iReflectionHighHBlue, iReflectionHighSBlue,
                 iReflectionHighVBlue),
      blueImgInRangeReflection);

  // Use bitwise AND operator to find reflections in blue cone image
  cv::bitwise_and(blueImgInRange, blueImgInRangeReflection, blueImgAnd);

  // Use bitwise XOR operator to remove found reflections from blue cone image
  cv::bitwise_xor(blueImgAnd, blueImgInRange, finalBlueImg);

  std::pair<cv::Mat, cv::Mat> pairs =
      std::make_pair(yellowImgInRange, finalBlueImg);

  return pairs;
}