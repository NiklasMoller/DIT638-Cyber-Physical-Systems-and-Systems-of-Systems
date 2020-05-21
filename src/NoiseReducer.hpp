#ifndef NOISEREDUCER
#define NOISEREDUCER

// Include the single-file, header-only middleware libcluon to create
// high-performance microservices
#include "cluon-complete-v0.0.127.hpp"

// Include the GUI and image processing header files from OpenCV
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class NoiseReducer
{
  public:
    std::pair<cv::Mat, cv::Mat> processImage(std::pair<cv::Mat, cv::Mat> image);
};

#endif