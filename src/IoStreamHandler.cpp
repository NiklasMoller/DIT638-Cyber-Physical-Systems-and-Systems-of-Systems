#include <fstream>
#include <iostream>

#include "IoStreamHandler.hpp"

std::ofstream myfile;
int firstTime = 0;

void IoStreamHandler::openOutputStream(uint32_t iteration) {

  // get iteration file name
  std::string iteration_string = std::to_string(iteration);
  std::string filename = "data/group10_" + iteration_string + ".csv";

  if (firstTime == 0) {
    myfile.open(filename, std::ios::trunc); /* Flawfinder: ignore */ // NOLINT
    myfile << "TimeStamp, GroundSteeringRequest, \n";
    firstTime++;
  } else {

    myfile.open(filename, std::ios::app); /* Flawfinder: ignore */ // NOLINT
    firstTime++;
  }
}

void IoStreamHandler::closeOutputStream() { myfile.close(); }

void IoStreamHandler::writeVehicleData(cluon::data::TimeStamp ts) {

  int64_t ts2 = cluon::time::toMicroseconds(ts);
  std::string timeStamp = std::to_string(ts2);

  myfile << timeStamp + ",";
}

void IoStreamHandler::writeGroundSteeringRequest(float groundSteeringRequest) {

  std::string computedGroundSteeringRequest =
      std::to_string(groundSteeringRequest);

  myfile << computedGroundSteeringRequest + ", \n";
}
