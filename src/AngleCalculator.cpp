#define PI 3.1415926535
#define X_POSITION_OF_CAR 320
#define Y_POSITION_OF_CAR 480

#include "AngleCalculator.hpp"

// Include the single-file, header-only middleware libcluon to create
// high-performance microservices
#include "cluon-complete-v0.0.127.hpp"

// Include the GUI and image processing header files from OpenCV
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

float AngleCalculator::distanceBetweenTwoPoints(float xInPoint1,
                                                float yInPoint1,
                                                float xInPoint2,
                                                float yInPoint2) {

  float distance = std::sqrt(std::pow(xInPoint2 - xInPoint1, 2) +
                             std::pow(yInPoint2 - yInPoint1, 2) * 1.0);
  return distance;
}

float AngleCalculator::getAngleBetweenCones(std::vector<cv::Point2f> points) {

  // Detecting the two mass centres closest to the car by sorting the vector by
  // its y values Credits:
  // https://stackoverflow.com/questions/18253065/is-there-any-function-to-sort-a-stdvector-cvpoint2f-vector
  std::sort(points.begin(), points.end(),
            [](const cv::Point2f &a, const cv::Point2f &b) {
              return a.y > b.y; // Sorting in order from highest to lowest
            });

  float distance = distanceBetweenTwoPoints(
      X_POSITION_OF_CAR, Y_POSITION_OF_CAR, points[0].x, points[0].y);

  if (distance < 200) {
    // Getting the angle in radians
    float radians =
        std::atan2(points[0].y - points[1].y, points[0].x - points[1].x);

    // Converting to degrees
    float degrees = radians * 180 / PI;

    // Adjusting so the value is measured from right x axis
    float adjusted = 180 - degrees;

    if (adjusted > 60 && adjusted < 120) {
      return 90;
    } else {
      return adjusted;
    }
  }

  return 90;
}

float AngleCalculator::getAngleOfRoad(std::vector<cv::Point2f> yellow,
                                      std::vector<cv::Point2f> blue) {

  float yellowAngle;
  float blueAngle;

  // If there are at least 2 blue and 2 yellow cones
  if ((yellow.size() >= 2) && (blue.size() >= 2)) {

    yellowAngle = getAngleBetweenCones(yellow);
    blueAngle = getAngleBetweenCones(blue);

    // Taking the mean value of the found angles
    float meanAngle = (yellowAngle + blueAngle) / 2;

    return meanAngle;
  } else if (yellow.size() >= 2) {

    yellowAngle = getAngleBetweenCones(yellow);

    return yellowAngle;
  } else if (blue.size() >= 2) {

    blueAngle = getAngleBetweenCones(blue);

    return blueAngle;
  } else {

    return 90;
  }
}
