cmake_minimum_required(VERSION 3.6)
project(TNCG15_Project)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BINARY_DIR   ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH    ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR})

set(GLM_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/libs)
include_directories(${GLM_INCLUDE_DIRS})


set(SOURCE_FILES main.cpp ColorDbl.h Ray.h Triangle.h Scene.h Camera.h Pixel.h Sphere.h Renderer.h)
add_executable(TNCG15_Project ${SOURCE_FILES})