cmake_minimum_required(VERSION 3.14)
project(SwitchDetectionNew)

find_package(OpenCV REQUIRED)
set(CMAKE_CXX_STANDARD 14)

add_executable(SwitchDetectionNew main.cpp)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(SwitchDetectionNew ${OpenCV_LIBS})