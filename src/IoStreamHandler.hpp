#ifndef IOSTREAMHANDLER
#define IOSTREAMHANDLER

// Include the single-file, header-only middleware libcluon to create
// high-performance microservices
#include "cluon-complete-v0.0.127.hpp"


class IoStreamHandler{

    public: 
        void openOutputStream(uint32_t iteration);
        void closeOutputStream();
        void writeVehicleData(cluon::data::TimeStamp ts);
        void writeGroundSteeringRequest(float groundSteeringRequest);

};

#endif