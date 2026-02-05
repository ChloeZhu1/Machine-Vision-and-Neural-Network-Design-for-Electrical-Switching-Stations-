cmake_minimum_required(VERSION 3.14)
project(TextRec)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)

# Tesseract + Leptonica are typically discoverable via pkg-config on macOS (brew) and Linux (apt).
# If pkg-config can't find them, install the dev packages and ensure PKG_CONFIG_PATH is set.
#   macOS: brew install tesseract leptonica pkg-config
#   Ubuntu: sudo apt-get install -y libtesseract-dev libleptonica-dev pkg-config
pkg_check_modules(TESSERACT REQUIRED tesseract)
pkg_check_modules(LEPTONICA REQUIRED lept)

add_executable(TextRec main.cpp)

target_include_directories(TextRec PRIVATE
  ${OpenCV_INCLUDE_DIRS}
  ${TESSERACT_INCLUDE_DIRS}
  ${LEPTONICA_INCLUDE_DIRS}
)

target_link_libraries(TextRec PRIVATE
  ${OpenCV_LIBS}
  ${TESSERACT_LIBRARIES}
  ${LEPTONICA_LIBRARIES}
)

# Some environments need this for pkg-config discovered libs
target_link_directories(TextRec PRIVATE ${TESSERACT_LIBRARY_DIRS} ${LEPTONICA_LIBRARY_DIRS})
