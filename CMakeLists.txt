cmake_minimum_required(VERSION 2.8.4)
project(Renbro)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(Renbro ${SOURCE_FILES})

add_library(TGA tgaimage.cpp)
target_link_libraries(Renbro TGA)

add_library(MODEL model.cpp)
target_link_libraries(Renbro MODEL)