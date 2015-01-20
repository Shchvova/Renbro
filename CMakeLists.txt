cmake_minimum_required(VERSION 2.8.4)
project(Renbro)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(RESOURCES_PATH "${PROJECT_SOURCE_DIR}/res/")
set(RESULTS_PATH "${PROJECT_SOURCE_DIR}/out/")

file(MAKE_DIRECTORY  ${RESULTS_PATH})

set(CONFIGURE_PATH "${PROJECT_SOURCE_DIR}/gen/")

configure_file (
  "${PROJECT_SOURCE_DIR}/configuration.h.in"
  "${CONFIGURE_PATH}/configuration.h"
)

include_directories("${CONFIGURE_PATH}")

set(SOURCE_FILES main.cpp model.cpp tgaimage.cpp geometry.h)
add_executable(Renbro ${SOURCE_FILES})

