# Switchboard Status Recognition (OpenCV + EAST + Tesseract)

A C++ computer-vision project for **switchboard/industrial panel monitoring**:

1) detect **red/green indicator lights** (circle/contour-based segmentation)
2) detect **text regions** with the EAST scene-text detector (OpenCV DNN)
3) run **OCR** (Tesseract, Chinese supported) on selected regions

This repo reorganizes the original bachelor-project附件 into a clean, reproducible GitHub layout.

## Repository layout

```
.
├── apps/
│   ├── switch_detection_new/          # Red/green indicator detection demos
│   ├── text_detection_east/           # EAST text detector (OpenCV DNN)
│   ├── text_recognition_tesseract/    # Tesseract OCR demo
│   └── bachelor_project/              # Combined demo (indicator + OCR)
├── assets/
│   ├── images/                        # Sample images used in testing
│   └── videos/                        # Sample videos (ignored by git; use Git LFS if needed)
├── models/
│   └── frozen_east_text_detection.pb  # EAST model
└── CMakeLists.txt                     # Top-level build
```

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

## Notes / TODOs

- Some original demo code was written with hard-coded local file paths; this repo replaces the defaults with `assets/...` paths.
- If you want to commit large videos, use **Git LFS** and remove the `*.mp4` ignore rule.
- Next cleanup targets (recommended):
  - unify CLI arguments across apps
  - package indicator detection into a reusable library module
  - add non-interactive OCR mode (auto-crop via EAST)

---

If you want, tell me your preferred repo name and a one-sentence project tagline; I can also generate:
- a polished `docs/` write-up (method + results)
- an SVG pipeline diagram for your README
- GitHub release-ready screenshots / GIFs
