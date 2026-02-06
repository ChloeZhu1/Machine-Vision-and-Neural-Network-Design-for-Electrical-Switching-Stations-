cmake_minimum_required(VERSION 3.14)
project(switch_vision_ocr LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build individual demo apps.
add_subdirectory(apps/text_detection_east)
add_subdirectory(apps/text_recognition_tesseract)
add_subdirectory(apps/switch_detection_new)
add_subdirectory(apps/bachelor_project)
