cmake_minimum_required(VERSION 3.3)

set(GLUT_INCLUDE_DIR "/opt/X11/include")
set(OPENGL_LIBRARY_DIR "/opt/X11/include")

project(food_fighter)
include_directories(include)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB C_SOURCE_FILES "src/*.c")
file(GLOB CPP_SOURCE_FILES "src/*.cpp")

message(WARNING ${CPP_SOURCE_FILES})


set(OpenCV_DIR [OpenCV.cmake PATH])

find_package(OpenCV REQUIRED)

#find_package(dlib REQUIRED)
#add_definitions(-DDLIB_USE_CUDA)

find_package(SDL2 REQUIRED)

find_package(OpenGL REQUIRED)

find_package(GLUT REQUIRED)

include_directories(${OpenCV_INCLUDE_DIR}  ${OPENGL_INCLUDE_DIR}  ${GLUT_INCLUDE_DIR})

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )


#set(SOURCE_FILES facialLandmarkDetection.cpp drawLandmarks.hpp renderFace.hpp)

add_executable(food_fighter ${C_SOURCE_FILES} ${CPP_SOURCE_FILES})


#${dlib_LIBRARIES}
target_link_libraries(food_fighter ${OpenCV_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY}  ${SDL2_LIBRARIES})
