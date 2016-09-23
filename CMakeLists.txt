cmake_minimum_required(VERSION 3.6)
project(TNCG15_proj)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BINARY_DIR   ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH    ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR})

set(SOURCE_FILES main.cpp Renderer.cpp Renderer.h Pixel.cpp Pixel.h Ray.cpp Ray.h ColorDbl.cpp ColorDbl.h Scene.cpp Scene.h Triangle.cpp Triangle.h Sphere.cpp Sphere.h Camera.cpp Camera.h)
add_executable(TNCG15_proj ${SOURCE_FILES})