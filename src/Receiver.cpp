#include "Receiver.hpp"
#include "AngleCalculator.hpp"
#include "ColourProcessor.hpp"
#include "ContourFinder.hpp"
#include "DecisionSender.hpp"
#include "IoStreamHandler.hpp"
#include "NoiseReducer.hpp"

// Include the single-file, header-only middleware libcluon to create
// high-performance microservices
#include "cluon-complete-v0.0.127.hpp"

// Include the OpenDLV Standard Message Set that contains messages that are
// usually exchanged for automotive or robotic applications
#include "opendlv-standard-message-set.hpp"

// Include the GUI and image processing header files from OpenCV
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void Receiver::start(int cid, std::unique_ptr<cluon::SharedMemory> sharedMemory,
                     const uint32_t WIDTH, const uint32_t HEIGHT,
                     const bool VERBOSE, const uint32_t ITERATION) {
  ColourProcessor colourProcessor;
  ContourFinder contourFinder;
  DecisionSender decisionSender;
  NoiseReducer noiseReducer;
  AngleCalculator angleCalculator;
  IoStreamHandler ioStreamHandler;

  // Interface to a running OpenDaVINCI session where network messages are
  // exchanged. The instance od4 allows you to send and receive messages.
  cluon::OD4Session od4{static_cast<uint16_t>(cid)};

  // Receive pedal position message
  opendlv::proxy::PedalPositionRequest ppr;
  std::mutex pprMutex;
  auto onPedalPositionRequest = [&ppr, &pprMutex](cluon::data::Envelope &&env) {
    std::lock_guard<std::mutex> lck(pprMutex);
    ppr = cluon::extractMessage<opendlv::proxy::PedalPositionRequest>(
        std::move(env));
    // std::cout << "lambda: pedalposition = " << ppr.position() << std::endl;
  };

  od4.dataTrigger(opendlv::proxy::PedalPositionRequest::ID(),
                  onPedalPositionRequest);

  // Endless loop; end the program by pressing Ctrl-C.
  while (od4.isRunning()) {

    // OpenCV data structure to hold an image.
    cv::Mat img;

    // init of matrix for debug window
    cv::Mat debug_window(512, 512, CV_8UC3, cv::Scalar(0));

    // Wait for a notification of a new frame.
    sharedMemory->wait();

    sharedMemory->lock();
    {
      // Copy the pixels from the shared memory into our own data structure.
      cv::Mat wrapped(HEIGHT, WIDTH, CV_8UC4, sharedMemory->data());
      img = wrapped.clone();
    }

    // Sending timeStamp to IoStreamHandler
    std::pair<bool, cluon::data::TimeStamp> ts = sharedMemory->getTimeStamp();

    int64_t ts2 = cluon::time::toMicroseconds(ts.second);
    std::string ts_string = std::to_string(ts2);

    sharedMemory->unlock();

    // after receving image
    cv::putText(debug_window,                           // target image
                "NEW FRAME",                            // text
                cv::Point(10, debug_window.rows - 100), // top-left position
                cv::FONT_HERSHEY_PLAIN, 1.0,
                CV_RGB(255, 255, 255), // font color
                2);

    // set timestamp string
    std::string timestamp_string = "Timestamp: " + ts_string;

    // after receving image
    cv::putText(debug_window,                          // target image
                timestamp_string,                      // text
                cv::Point(10, debug_window.rows - 80), // top-left position
                cv::FONT_HERSHEY_PLAIN, 1.0,
                CV_RGB(255, 255, 255), // font color
                2);

    float pedalPos;

    // Access the latest received pedal position and lock the mutex
    {
      std::lock_guard<std::mutex> lck(pprMutex);
      pedalPos = ppr.position();
    }

    cv::Mat original = img.clone();
    std::pair<cv::Mat, cv::Mat> processedImage =
        colourProcessor.processImage(img);
    processedImage = noiseReducer.processImage(processedImage);

    // set string
    std::string height = std::to_string(processedImage.first.size().height);
    std::string width = std::to_string(processedImage.first.size().width);
    std::string colourProcessor_string =
        "Image has height: " + height + ". Width: " + width;

    // after colourProcessor
    cv::putText(debug_window,                         // target image
                colourProcessor_string,               // text
                cv::Point(4, debug_window.rows - 60), // top-left position
                cv::FONT_HERSHEY_PLAIN, 1.0,
                CV_RGB(255, 255, 255), // font color
                2);

    // Finding the coordinates of the mass centre points
    // Putting these as yellow and blue vector into a pair called
    // massCentreCoordinates
    std::pair<std::vector<cv::Point2f>, std::vector<cv::Point2f>>
        massCentreCoordinates =
            contourFinder.findContours(processedImage, original);

    // set string
    std::string nr_of_yellow_cones =
        std::to_string(massCentreCoordinates.first.size());
    std::string nr_of_blue_cones =
        std::to_string(massCentreCoordinates.second.size());
    std::string contourfinder_string = "Found " + nr_of_yellow_cones +
                                       " of yellow cones. Found " +
                                       nr_of_blue_cones + " of blue cones.";

    // after contourFinder
    cv::putText(debug_window,                         // target image
                contourfinder_string,                 // text
                cv::Point(6, debug_window.rows - 40), // top-left position
                cv::FONT_HERSHEY_PLAIN, 1.0,
                CV_RGB(255, 255, 255), // font color
                2);

    // Calculate the angle of the road based on the masscentre points
    float angleOfRoad = angleCalculator.getAngleOfRoad(
        massCentreCoordinates.first, massCentreCoordinates.second);

    // set string
    std::string angle_string = std::to_string(angleOfRoad);
    std::string anglecalculator_string =
        "Angle for calculation is: " + angle_string;

    // after angleCalculator
    cv::putText(debug_window,                         // target image
                anglecalculator_string,               // text
                cv::Point(8, debug_window.rows - 20), // top-left position
                cv::FONT_HERSHEY_PLAIN, 1.0,
                CV_RGB(255, 255, 255), // font color
                2);

    // Calculate the groundSteeringRequest
    float groundSteeringRequest =
        decisionSender.computeGroundSteeringRequest(angleOfRoad, pedalPos);
    ioStreamHandler.openOutputStream(ITERATION);
    ioStreamHandler.writeVehicleData(ts.second);
    ioStreamHandler.writeGroundSteeringRequest(groundSteeringRequest);

    // Closing outputstream
    ioStreamHandler.closeOutputStream();

    // Sending the decision to the car
    decisionSender.sendGroundSteeringRequest(groundSteeringRequest, ts);

    // Check if verbose
    if (VERBOSE) {
      // Create debug window
      cv::namedWindow("DEBUG WINDOW", CV_WINDOW_AUTOSIZE);
      cv::imshow("DEBUG WINDOW", debug_window);
    }
  }
}