#ifndef PIPE
#define PIPE

// Include the single-file, header-only middleware libcluon to create
// high-performance microservices
#include "cluon-complete-v0.0.127.hpp"

// Include the GUI and image processing header files from OpenCV
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Pipe
{
public:
  void open(cv::Mat image);
  cv::Mat forward(cv::Mat image);
};

#endif