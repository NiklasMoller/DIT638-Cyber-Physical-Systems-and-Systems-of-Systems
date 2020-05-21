#include "NoiseReducer.hpp"

std::pair<cv::Mat, cv::Mat>
NoiseReducer::processImage(std::pair<cv::Mat, cv::Mat> image) {
  std::pair<cv::Mat, cv::Mat> reducedImg;

  // fill holes in objects - yellow cones
  cv::dilate(image.first, reducedImg.first,
             cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(8, 8)));
  cv::erode(reducedImg.first, reducedImg.first,
            cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(8, 8)));

  // remove small objects - yellow cones
  cv::erode(reducedImg.first, reducedImg.first,
            cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
  cv::dilate(reducedImg.first, reducedImg.first,
             cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7)));

  // fill holes in objects - blue cones
  cv::dilate(image.second, reducedImg.second,
             cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(8, 8)));
  cv::erode(reducedImg.second, reducedImg.second,
            cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(8, 8)));

  // remove small objects - blue cones
  cv::erode(reducedImg.second, reducedImg.second,
            cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
  cv::dilate(reducedImg.second, reducedImg.second,
             cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7)));

  // show windows
  /*
  cv::namedWindow("reducedimage yellow", CV_WINDOW_AUTOSIZE);
  cv::imshow("reducedimage yellow", reducedImg.first);

  cv::namedWindow("reducedimage blue", CV_WINDOW_AUTOSIZE);
  cv::imshow("reducedimage blue", reducedImg.second);
  */

  return reducedImg;
}