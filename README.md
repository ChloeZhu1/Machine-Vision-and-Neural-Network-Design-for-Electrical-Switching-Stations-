# Machine Vision and Neural Network Design for Electrical Switching Stations (OpenCV + EAST + Tesseract)

## Overview

This project implements an end-to-end computer vision OCR pipeline for reading switchboard and instrument panel text from images and video. It combines OpenCV-based preprocessing with the EAST deep learning text detector to localize text regions, followed by Tesseract OCR for recognition. The system extracts candidate regions, performs ROI refinement, and outputs annotated frames and recognized text. Designed as a practical, modular C++ demo for real-world panel and status-label reading.

A C++ computer-vision OCR for reading switch labels and status texts reliably on real-world **switchboard/industrial panels** in electrical stations:

1) detect **red/green indicator lights** (circle/contour-based segmentation)
2) detect **text regions** with the EAST scene-text detector (OpenCV DNN)
3) run **OCR** (Tesseract, Chinese supported) on selected regions

## Features

* C++ implementation with modular pipeline structure
* Image and video frame support
* OpenCV preprocessing (resize, normalize, denoise, contrast)
* EAST deep neural network text detection
* Non-maximum suppression for box filtering
* ROI extraction and deskewing
* Tesseract OCR recognition
* Annotated output images and frames
* Debug visualization outputs
* Clean CMake build configuration
  
## Repository layout

```
Machine Vision and Neural Network Design for Electrical Switching Stations (OpenCV + EAST + Tesseract)
.
├── apps/
│   ├── switch_status_inspection_image_proc+OCR/        # Red/green indicator detection, and text detection and recognition combined demos
│   ├── switch_status_inspection_image_proc/    # image processing
│   ├── text_detection_east/           # EAST text detector (OpenCV DNN)
│   └── text_recognition_tesseract/    # Tesseract OCR design         
├── assets/
│   ├── images/                        # Sample images used in testing
│   ├── download.svg                   # Pipeline diagram
│   └── videos/                        # Sample videos (ignored by git; use Git LFS if needed)
├── models/
│   └── frozen_east_text_detection.pb  # EAST model
├── CMakeLists.txt                # Top-level build
├── .gitignore
├── README.md
└── LICENSE
```

## Pipeline
![Pipeline](assets/download.svg)

## Dependencies

- CMake (>= 3.14)
- OpenCV (>= 4 recommended)
- Tesseract + Leptonica (for OCR apps)
- pkg-config (recommended for portable builds)

### macOS (Homebrew)
```bash
brew install cmake pkg-config opencv tesseract leptonica
```

### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install -y cmake pkg-config libopencv-dev libtesseract-dev libleptonica-dev
```

> Note: For Chinese OCR, install the `chi_sim` language data for Tesseract (package name varies by OS).

## Build

From the repo root:

```bash
mkdir -p build
cmake -S . -B build
cmake --build build -j
```

Binaries will be in `build/apps/...`.

## Run examples

### 1) Indicator light detection (red/green)
```bash
./build/apps/switch_detection_new/SwitchDetectionNew assets/images/IMG_5861.JPG
```

### 2) Text detection (EAST)
Run from the repo root (so `models/...` resolves), or pass an explicit `--model` path:

```bash
./build/apps/text_detection_east/TextDetection \
  --input assets/videos/your_video.mp4 \
  --model models/frozen_east_text_detection.pb
```

### 3) OCR (Tesseract)
```bash
./build/apps/text_recognition_tesseract/TextRec assets/images/T1.JPG
```

### 4) Combined demo (indicator visualization + OCR)
```bash
./build/apps/bachelor_project/BachelorProject assets/images/T1.JPG
```

## Applications

* Industrial panel monitoring
* Switchboard status reading
* Instrument label extraction
* Structured scene text recognition
* Vision-based inspection workflows
