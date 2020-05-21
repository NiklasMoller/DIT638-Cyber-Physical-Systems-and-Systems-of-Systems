#include "ContourFinder.hpp"
#include "ViewChanger.hpp"

std::pair<std::vector<cv::Point2f>, std::vector<cv::Point2f>>
ContourFinder::findContours(std::pair<cv::Mat, cv::Mat> image,
                            cv::Mat original) {
  ViewChanger viewChanger;
  const float CLUSTER_RANGE = 100.0;

  cv::Mat birdEyeView = viewChanger.changeView(original);

  // YELLOW
  std::vector<std::vector<cv::Point>> contours_yel;
  std::vector<cv::Vec4i> hierarchy_yel;
  cv::findContours(image.first, contours_yel, hierarchy_yel, CV_RETR_TREE,
                   CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

  // Get the moments
  std::vector<cv::Moments> mu_yel(contours_yel.size());
  for (unsigned int i = 0; i < contours_yel.size(); i++) {
    mu_yel[i] = moments(contours_yel[i], false);
  }

  // Get the mass centers:
  std::vector<cv::Point2f> mc_yel(contours_yel.size());
  std::vector<cv::Point2f> mc_transformed_yel(contours_yel.size());
  for (unsigned int i = 0; i < contours_yel.size(); i++) {
    mc_yel[i] = cv::Point2f(mu_yel[i].m10 / mu_yel[i].m00,
                            mu_yel[i].m01 / mu_yel[i].m00);
    mc_yel[i].y = mc_yel[i].y + 240;
    mc_transformed_yel = viewChanger.convertPoints(mc_yel);
  }


  // check if need to check clusters
  if (mc_transformed_yel.size() >= 2) {

    // fix clusters
    for (unsigned int i = 0; i < (mc_transformed_yel.size() - 1); i++) {
      float y_value = mc_transformed_yel[i].y - mc_transformed_yel[i + 1].y;

      if (y_value < CLUSTER_RANGE) {
        float x_value = mc_transformed_yel[i].x - mc_transformed_yel[i + 1].x;

        if (x_value < CLUSTER_RANGE) {
          // if close, remove then one from the top of the frame
          mc_transformed_yel.erase(mc_transformed_yel.begin() + i);
          i--;
        }
      }
    }
  }

  // Draw contours
  for (unsigned int i = 0; i < contours_yel.size(); i++) {
    cv::circle(birdEyeView, mc_transformed_yel[i], 10, cv::Scalar(0, 0, 255), 1,
               CV_AA, 0);
  }

  // BLUE
  std::vector<std::vector<cv::Point>> contours_blue;
  std::vector<cv::Vec4i> hierarchy_blue;
  cv::findContours(image.second, contours_blue, hierarchy_blue, CV_RETR_TREE,
                   CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

  // Get the moments
  std::vector<cv::Moments> mu_blue(contours_blue.size());
  for (unsigned int i = 0; i < contours_blue.size(); i++) {
    mu_blue[i] = moments(contours_blue[i], false);
  }

  //  Get the mass centers:
  std::vector<cv::Point2f> mc_blue(contours_blue.size());
  std::vector<cv::Point2f> mc_transformed_blue(contours_blue.size());
  for (unsigned int i = 0; i < contours_blue.size(); i++) {
    mc_blue[i] = cv::Point2f(mu_blue[i].m10 / mu_blue[i].m00,
                             mu_blue[i].m01 / mu_blue[i].m00);
    mc_blue[i].y = mc_blue[i].y + 240;
    mc_transformed_blue = viewChanger.convertPoints(mc_blue);
  }

  // check if need to check clusters
  if (mc_transformed_blue.size() >= 2) {

    // fix clusters
    for (unsigned int i = 0; i < (mc_transformed_blue.size() - 1); i++) {
      float y_value = mc_transformed_blue[i].y - mc_transformed_blue[i + 1].y;

      if (y_value < CLUSTER_RANGE) {
        float x_value = mc_transformed_blue[i].x - mc_transformed_blue[i + 1].x;

        if (x_value < CLUSTER_RANGE) {
          // if close, remove then one from the top of the frame
          mc_transformed_blue.erase(mc_transformed_blue.begin() + i);
          i--;
        }
      }
    }
  }

  // Draw contours
  for (unsigned int i = 0; i < contours_blue.size(); i++) {
    cv::circle(birdEyeView, mc_transformed_blue[i], 10, cv::Scalar(0, 0, 255),
               1, CV_AA, 0);
  }
  // Suggestion: Instead of creating a pair, pass mc_transformed_yel and
  // mc_transformed_blue to getAngleOfRoad in AngleCalculator.cpp
  std::pair<std::vector<cv::Point2f>, std::vector<cv::Point2f>> coordinates =
      std::make_pair(mc_transformed_yel, mc_transformed_blue);

  // cv::namedWindow("contours", CV_WINDOW_AUTOSIZE);
  // cv::imshow("contours", birdEyeView);
  // cv::waitKey(1);

  return coordinates;
}