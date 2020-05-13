# DIT638 Cyber Physical Systems & Systems of Systems

## What?
A microservice performing image recogniztion for a self driving vechile.

## Why?
To detect the road ahead and steer the miniature vechile accordingly.

## How?
The application is developed for the [Kiwi platform](https://github.com/chalmers-revere/opendlv-tutorial-kiwi) and runs as a seperate microservice. It uses [libcluon](https://github.com/chrberger/libcluon) to connect to a shared memory where image data is received. It  performs object detection using [OpenCV](https://opencv.org/), discovering centre points of blue and yellow cones ahead. It then relies on trignnometric functions to compute an angle of the road that is translated into a matching steering request for the Kiwi car.

## Software Design
The application decomposes the task of image recognition using [Pipe and Filters](https://docs.microsoft.com/en-us/azure/architecture/patterns/pipes-and-filters). The filters perform Color Processing, Noise Reduction, Contour Finding and View Changes as well as calculations for the car.

<img src="SoftwareArchitecture.png" width="500">

## Technologies used for development
- [Linux](https://ubuntu.com/blog/tag/ubuntu-18-04)
- [Docker](https://www.docker.com/)
- [Cmake](https://cmake.org/)
- [Open CV](https://opencv.org/)
- C++

## Testing and Continuous Integration
- Unit tests has been written using the C++ testing library [Catch2](https://github.com/catchorg/Catch2).

- Using [GitLab CI/CD](https://docs.gitlab.com/ee/ci/), a test coverage report is generated automatically on a successful build. The test coverage reports is created by adding the GCOV flag to the compilation, running the unit tests and then running a bash script with LCOV on the .gcda files from the compilation. The html files which was the outcome from running LCOV is then merged within GitLab pipeline into a pdf that can be displayed as an artifact on the Gitlab repository.  

- Automatic code evaluation and security checks has been performed with [Flawfinder](https://dwheeler.com/flawfinder/), [Scan Build](https://clang-analyzer.llvm.org/scan-build.html) and [PMCCABE](https://people.debian.org/~bame/pmccabe/overview.html)

- Acceptance Testing has been generated using Python3 and [Matlibplot libary](https://matplotlib.org/).

## Developers
- [Emanuel Dellsén](https://github.com/EmanuelDellsen)
- [Susan Lieu](https://github.com/SusanLieu)
- [Niklas Möller](https://github.com/NiklasMoller)
- [Karl Westgårdh](https://github.com/KarlWestgardh)

## Running the application
