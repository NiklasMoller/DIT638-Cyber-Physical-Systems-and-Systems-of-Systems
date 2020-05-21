# CMake version check
cmake_minimum_required(VERSION 3.2)

# Name of project
project(helloworld)

# Enable C++ standard version
set(CMAKE_CXX_STANDARD 11)

# Flags used by make when compiling
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static") #used between ubuntu and scratch
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror") #enable warnings and set to errors
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage") #for code coverage

#Files to be compiled:
add_executable(${PROJECT_NAME} 

    #  add more files to be executed here:
    ${CMAKE_CURRENT_SOURCE_DIR}/helloworld.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/PrimeChecker.cpp

    )

# Enables testing for this directory and below.
enable_testing()

# Files to be tested (compiled first then tested):
add_executable(

    # add files to be tested and :
    ${PROJECT_NAME}-Runner TestPrimeChecker.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/PrimeChecker.cpp
    
    )
add_test(

    NAME ${PROJECT_NAME}-Runner 
    COMMAND ${PROJECT_NAME}-Runner
    
    )