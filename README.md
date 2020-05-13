# DIT638 Cyber Physical Systems & Systems of Systems

## What?
A microservice performing image recogniztion for a self driving vechile.

## Why?
To detect the road ahead and steer the car accordingly.

## How?
The application is developed for the [Kiwi platform](https://github.com/chalmers-revere/opendlv-tutorial-kiwi) and runs as a seperate microservice. It uses [libcluon](https://github.com/chrberger/libcluon) to connect to a shared memory with image data and performs image recogniztion using [OpenCV](https://opencv.org/). It discovers centre points of cones ahead and uses simple trignnometric functions to compute the angle of the road which is translated into a matching steering request for the Kiwi car.

## Software Design
The application decomposes the task of image recognition using [Pipe and Filters](https://docs.microsoft.com/en-us/azure/architecture/patterns/pipes-and-filters). The filters perform Color Processing, Noise Reduction, Contour Finding and View Changes.

## Technologies used
- Linux
- Docker
- C++
- Catch
- Python3
- Open CV

## Testing
Unit test have been written in Catch
Acceptance Testing has been performed with Python3.

## Developers
- [Emanuel Dellsén](https://github.com/EmanuelDellsen)
- [Susan Lieu](https://github.com/SusanLieu)
- [Niklas Möller](https://github.com/NiklasMoller)
- [Karl Westgårdh](https://github.com/KarlWestgardh)


