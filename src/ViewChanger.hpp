#ifndef VIEWCHANGER
#define VIEWCHANGER

// Include the single-file, header-only middleware libcluon to create
// high-performance microservices
#include "cluon-complete-v0.0.127.hpp"

// Include the GUI and image processing header files from OpenCV
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class ViewChanger
{
  public:
    cv::Mat calculateMatrix();
    cv::Mat changeView(cv::Mat image);
    std::vector<cv::Point2f> convertPoints( std::vector<cv::Point2f> coordinates);
};

#endif