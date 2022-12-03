<p align="center"><img src="https://raw.githubusercontent.com/Karaka-Management/Assets/master/art/logo.png" width="256" alt="Logo"></p>

The Bubble Detector is a command line tool for recognizing bubbles in fluids.

Recognition features include defining threasholds in bubble size, bubble quantity, total bubble volume and highlighting potential bubbles in the provided image.

## Dependencies

* Windows 7 or newer
* tesseract-ocr (or other) for text recognition
* pdftoppm (or other) for pdf to image export if your scans are pdf

### Developer tools

* Language: C++
* Libraries: opencv
* Tools: CMake or Visual Studio

## Installation

Simply download the .exe and you can run the program

## Usage

The default usage to automatically run all possible optimizations is:

```sh
OCRImageOptimizerApp.exe -i <INPUT_IMAGE> -o <OUTPUT_IMAGE>
```

### Arguments

* `-h` Show help
* `-i` Input image
* `-o` Output image
* `--rotate` Tries to automatically fix the image rotation
* `--edges` Tries to automatically detect and remove irrelevant parts from the image
* `--binary` Tries to turn the image purely into black and white and remove shadows

## Images

<p align="center"><img src="img/OCRImageOptimizer_splash.png" alt="UI"></p>

## Legal

* [Terms of Service](https://jingga.app/en/terms)
* [License](https://github.com/Karaka-Management/OCRImageOptimizerApp/blob/master/LICENSE.txt)

### Disclaimer

You can use the demo application for some tests in order to see if the software fulfills your needs.

> The demo application prints a water mark on the image.
