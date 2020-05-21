#include "ViewChanger.hpp"

// Include the single-file, header-only middleware libcluon to create
// high-performance microservices
#include "cluon-complete-v0.0.127.hpp"

// Include the GUI and image processing header files from OpenCV
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat ViewChanger::calculateMatrix() {

  //  Calibration vertices
  //
  //    [0]    [1]
  //
  //  [2]         [3]
  //
  // These map to the corners of the checkerboard, see:
  // /assets/images/calibration.png
  cv::Point2f src_vertices[4];
  src_vertices[0] = cv::Point(207, 285);
  src_vertices[1] = cv::Point(364, 285);
  src_vertices[2] = cv::Point(476, 350);
  src_vertices[3] = cv::Point(89, 353);

  // How the vertices should be mapped on the destination image
  //
  //    [0]   [1]
  //
  //    [2]   [3]
  //

  cv::Point2f dst_vertices[4];

  // CLOSE PERSPECTIVE
  dst_vertices[0] = cv::Point(125, 130);
  dst_vertices[1] = cv::Point(390, 130);
  dst_vertices[2] = cv::Point(390, 395);
  dst_vertices[3] = cv::Point(125, 395);

  /* MEDIUM PERSPECTIVE
  dst_vertices[0] = Point(150, 105);
  dst_vertices[1] = Point(415, 105);
  dst_vertices[2] = Point(415, 370);
  dst_vertices[3] = Point(150, 370);
  */

  /* FAR PERSPECTIVE
  dst_vertices[0] = Point(200, 265);
  dst_vertices[1] = Point(365, 265);
  dst_vertices[2] = Point(365, 430);
  dst_vertices[3] = Point(200, 430);
  */

  // Calculates a matrix of a perspective transform
  cv::Mat M = getPerspectiveTransform(src_vertices, dst_vertices);

  return M;
}

cv::Mat ViewChanger::changeView(cv::Mat image) {
  // Size of destination image
  cv::Mat dst(480, 640, CV_8UC3);

  cv::Mat M = calculateMatrix();

  // Changing perspective to birds-eye view
  cv::warpPerspective(image, dst, M, dst.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);

  // cv::imshow("birds-eye-view", dst);

  cv::waitKey(1);

  return dst;
}

std::vector<cv::Point2f>
ViewChanger::convertPoints(std::vector<cv::Point2f> coordinates) {
  cv::Mat M = calculateMatrix();

  std::vector<cv::Point2f> dst_points;

  // Changing perspective to birds-eye view
  cv::perspectiveTransform(coordinates, dst_points, M);
  return dst_points;
}