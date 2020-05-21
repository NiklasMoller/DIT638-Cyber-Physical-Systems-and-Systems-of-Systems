#ifndef RECEIVER
#define RECEIVER

// Include the single-file, header-only middleware libcluon to create
// high-performance microservices
#include "cluon-complete-v0.0.127.hpp"

class Receiver
{

public:
    void start(int cid, std::unique_ptr<cluon::SharedMemory> sharedMemory, const uint32_t WIDTH,
               const uint32_t HEIGHT, const bool VERBOSE, const uint32_t ITERATION);
};

#endif