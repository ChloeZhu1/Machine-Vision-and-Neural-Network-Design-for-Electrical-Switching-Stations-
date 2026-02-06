# Machine Vision and Neural Network Design for Electrical Switching Stations (OpenCV + EAST + Tesseract)

A C++ computer-vision OCR for reading switch labels and status texts reliably on real-world **switchboard/industrial panels** in electrical stations:

1) detect **red/green indicator lights** (circle/contour-based segmentation)
2) detect **text regions** with the EAST scene-text detector (OpenCV DNN)
3) run **OCR** (Tesseract, Chinese supported) on selected regions

## Repository layout

```
.
├── apps/
│   ├── switch_status_inspection_image_proc+OCR/        # Red/green indicator detection, and text detection and recognition combined demos
│   ├── switch_status_inspection_image_proc/    # image processing
│   ├── text_detection_east/           # EAST text detector (OpenCV DNN)
│   └── text_recognition_tesseract/    # Tesseract OCR design         
├── assets/
│   ├── images/                        # Sample images used in testing
    ├── download.svg                   # Pipeline diagram
│   └── videos/                        # Sample videos (ignored by git; use Git LFS if needed)
├── models/
│   └── frozen_east_text_detection.pb  # EAST model
└── CMakeLists.txt                     # Top-level build
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

> The OCR demos use an interactive ROI selector (`selectROI`)—you draw a box around the text you want recognized.
