cmake_minimum_required(VERSION 3.17)
project(Mask)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(deps/argparse)

add_executable(Test test.cpp libs/fileUtils.h libs/cvUtils.h libs/MessageReplacer.h)
add_executable(Train train.cpp libs/fileUtils.h libs/cvUtils.h libs/MessageReplacer.h)

target_link_libraries(Test ${OpenCV_LIBS})
target_link_libraries(Train ${OpenCV_LIBS})


# GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(executeTests tests/computeHistogram.cpp libs/cvUtils.h)
target_link_libraries(executeTests ${GTEST_LIBRARIES} pthread)