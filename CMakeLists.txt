cmake_minimum_required(VERSION 3.14)
project(waterLine)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

add_executable(waterLine main.cpp)

target_link_libraries(waterLine ${OpenCV_LIBS})