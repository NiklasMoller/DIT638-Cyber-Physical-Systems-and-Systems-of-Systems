#include "PrimeChecker.hpp"
#include <iostream>
#include <string.h>

int main(int argc, char **argv) { // this triggers scan-build error
  // int *pointer;
  // memset(pointer, 0, sizeof(pointer));
  // this triggers flawfinder and Wall
  // char buff[1024];
  // printf(buff);
  // add just for purpose of addressing errors g++ Wall
  std::cout << argv[0] << std::endl;

  if (argc == 2) {
    std::cout << "Karl Douglas" << std::endl;
    // changed to const int since it wont change and const takes less memory
    // const int number = std::stoi(argv[1]);
    // PrimeChecker pc;
    // std::cout << "Dellsén, Emanuel; " << number << " is a prime number? " <<
    // pc.isPrime(number) << std::endl;
  }
  return 0;
}