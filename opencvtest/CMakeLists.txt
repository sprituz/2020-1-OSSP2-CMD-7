cmake_minimum_required(VERSION 3.3)

project(food_fighter)



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")



set(OpenCV_DIR [OpenCV.cmake PATH])

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIR})



set(SOURCE_FILES facialLandmarkDetection.cpp drawLandmarks.hpp renderFace.hpp)

add_executable(food_fighter ${SOURCE_FILES})

target_link_libraries(food_fighter ${OpenCV_LIBRARIES})