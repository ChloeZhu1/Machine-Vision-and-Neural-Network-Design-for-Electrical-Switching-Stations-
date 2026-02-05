cmake_minimum_required(VERSION 3.14)
project(BachelorProject)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(TESSERACT REQUIRED tesseract)
pkg_check_modules(LEPTONICA REQUIRED lept)

add_executable(BachelorProject main.cpp)

target_include_directories(BachelorProject PRIVATE
  ${OpenCV_INCLUDE_DIRS}
  ${TESSERACT_INCLUDE_DIRS}
  ${LEPTONICA_INCLUDE_DIRS}
)

target_link_libraries(BachelorProject PRIVATE
  ${OpenCV_LIBS}
  ${TESSERACT_LIBRARIES}
  ${LEPTONICA_LIBRARIES}
)

target_link_directories(BachelorProject PRIVATE ${TESSERACT_LIBRARY_DIRS} ${LEPTONICA_LIBRARY_DIRS})
