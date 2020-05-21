#ifndef ANGLECALCULATOR
#define ANGLECALCULATOR

// Include the single-file, header-only middleware libcluon to create
// high-performance microservices
#include "cluon-complete-v0.0.127.hpp"

// Include the GUI and image processing header files from OpenCV
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


class AngleCalculator{

    public: 
        float getAngleBetweenCones(std::vector<cv::Point2f> points);
        float getAngleOfRoad(std::vector<cv::Point2f> yellow, std::vector<cv::Point2f> blue);
        float distanceBetweenTwoPoints(float xInPoint1, float yInPoint1, float xInPoint2, float yInPoint2);

};

#endif