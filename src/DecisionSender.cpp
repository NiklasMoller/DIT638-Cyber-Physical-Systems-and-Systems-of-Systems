#include "DecisionSender.hpp"

// Include the single-file, header-only middleware libcluon to create
// high-performance microservices
#include "cluon-complete-v0.0.127.hpp"

// Include the GUI and image processing header files from OpenCV
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "opendlv-standard-message-set.hpp"

// Calculating the ground steering request of car
// Can if needed be broken out to own class "SteeringRequestCalculator"
float DecisionSender::computeGroundSteeringRequest(float angleOfRoad,
                                                   float pedalPos) {

  float groundSteeringRequest;
  // float roadAngleToCSV = angleOfRoad;
  // Only sending ground steering request with angles greater than 100 degrees
  // or smaller than 80 degrees
  // values are based on reverseengineering christians groundsteering requests
  if (((angleOfRoad <= 89) || (angleOfRoad >= 91)) && pedalPos != -0) {

    // Getting a value between 0 and 0.6 based on the angle
    // Subtracting with 0.3 to send in the set range of the car
    groundSteeringRequest = (angleOfRoad * 0.003333) - 0.3;
  } else {
    groundSteeringRequest = 0;
  }

  return groundSteeringRequest;
}

void DecisionSender::sendGroundSteeringRequest(
    float groundSteeringRequest,
    std::pair<bool, cluon::data::TimeStamp> ts_pair) {

  // This is where call to send msg to car would be located

  // For this project, only print the timestamp and the ground steering request
  std::string gsr_string = std::to_string(groundSteeringRequest);
  int64_t ts2 = cluon::time::toMicroseconds(ts_pair.second);
  std::string ts_string = std::to_string(ts2);

  std::cout << "group_10;" << ts_string << ";" << gsr_string << ";"
            << std::endl;
}