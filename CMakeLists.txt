cmake_minimum_required(VERSION 2.8.4)
project(Renbro)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(RESOURCES_PATH "${PROJECT_SOURCE_DIR}/res/")

configure_file (
  "${PROJECT_SOURCE_DIR}/configuration.h.in"
  "${PROJECT_BINARY_DIR}/configuration.h"
)

include_directories("${PROJECT_BINARY_DIR}")

set(SOURCE_FILES model.cpp tgaimage.cpp main.cpp)
add_executable(Renbro ${SOURCE_FILES})

