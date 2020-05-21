# Copyright (C) 2020  Christian Berger
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.2)

project(steering-decider)

################################################################################
# Defining the relevant versions of OpenDLV Standard Message Set and libcluon.
# The OpenDLV Standard Message Set contains a set of messages usually used in automotive research project.
set(OPENDLV_STANDARD_MESSAGE_SET opendlv-standard-message-set-v0.9.6.odvd)
# libcluon is a small and portable middleware to easily realize high-performance microservices with C++: https://github.com/chrberger/libcluon
set(CLUON_COMPLETE cluon-complete-v0.0.127.hpp)

################################################################################
# Set the search path for .cmake files.
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" ${CMAKE_MODULE_PATH})

################################################################################
# This project requires C++14 or newer.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add further warning levels to increase the code quality.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
    -D_XOPEN_SOURCE=700 \
    -D_FORTIFY_SOURCE=2 \
    -O2 \
    -Wall -Wextra -Werror \
    --coverage")

# Build a static binary.
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# Threads are necessary for linking the resulting binaries as the network communication is running inside a thread.
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

################################################################################
# Extract cluon-msc from cluon-complete.hpp.
# cluon-msc is the message compiler that compiles a .odvd message specification into a header-only C++ file.
add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/cluon-msc
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/src/${CLUON_COMPLETE} ${CMAKE_BINARY_DIR}/cluon-complete.hpp
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_BINARY_DIR}/cluon-complete.hpp ${CMAKE_BINARY_DIR}/cluon-complete.cpp
    COMMAND ${CMAKE_CXX_COMPILER} -o ${CMAKE_BINARY_DIR}/cluon-msc ${CMAKE_BINARY_DIR}/cluon-complete.cpp -std=c++14 -pthread -D HAVE_CLUON_MSC
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/${CLUON_COMPLETE})

################################################################################
# Generate opendlv-standard-message-set.hpp from ${OPENDLV_STANDARD_MESSAGE_SET} file.
add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/opendlv-standard-message-set.hpp
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_BINARY_DIR}/cluon-msc --cpp --out=${CMAKE_BINARY_DIR}/opendlv-standard-message-set.hpp ${CMAKE_CURRENT_SOURCE_DIR}/src/${OPENDLV_STANDARD_MESSAGE_SET}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/${OPENDLV_STANDARD_MESSAGE_SET} ${CMAKE_BINARY_DIR}/cluon-msc)
# Add current build directory as include directory as it contains generated files.
include_directories(SYSTEM ${CMAKE_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

################################################################################
# Gather all object code first to avoid double compilation.
set(LIBRARIES Threads::Threads)

if(UNIX)
    if(NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
        find_package(LibRT REQUIRED)
        set(LIBRARIES ${LIBRARIES} ${LIBRT_LIBRARIES})
        include_directories(SYSTEM ${LIBRT_INCLUDE_DIR})
    endif()
endif()

# This project uses OpenCV for image processing.
find_package(OpenCV REQUIRED core highgui imgproc imgcodecs)
include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS})
set(LIBRARIES ${LIBRARIES} ${OpenCV_LIBS})

################################################################################
# get all cpp files
FILE(GLOB SRCFILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Create executable.
add_executable(${PROJECT_NAME} 
                ${SRCFILES})


target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

# Add dependency to OpenDLV Standard Message Set.
add_custom_target(generate_opendlv_standard_message_set_hpp DEPENDS ${CMAKE_BINARY_DIR}/opendlv-standard-message-set.hpp)
add_dependencies(${PROJECT_NAME} generate_opendlv_standard_message_set_hpp)

################################################################################
# Install executable.
install(TARGETS ${PROJECT_NAME} DESTINATION bin COMPONENT ${PROJECT_NAME})

################################################################################
# Enable unit testing
enable_testing()

# retrieve all necessary packages
find_package(OpenCV REQUIRED COMPONENTS opencv_core opencv_imgproc opencv_calib3d opencv_video opencv_features2d opencv_ml opencv_highgui opencv_objdetect opencv_imgcodecs)

# add executable target with <name> using <sources>
add_executable (${PROJECT_NAME}-runner 
                ${CMAKE_CURRENT_SOURCE_DIR}/src/ViewChanger.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/ContourFinder.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/ColourProcessor.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/AngleCalculator.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/DecisionSender.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/MathHelper.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/MainTest.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/TestColourProcessor.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/TestContourFinder.cpp
		        ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/TestAngleCalculator.cpp
		        ${CMAKE_CURRENT_SOURCE_DIR}/src/tests/TestDecisionSender.cpp
                )

# link the executable with <name> to the <libs> found in find_package above (?)
target_link_libraries (${PROJECT_NAME}-runner ${OpenCV_LIBS})

# add test with <name> using the executable after COMMAND with <name_of_executable>)
add_test(NAME ${PROJECT_NAME}-runner COMMAND ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-runner)
