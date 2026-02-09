# Image Processing Project in C++

A **console-based C++ image processing application** that allows loading, manipulating, and saving grayscale images (PGM format). The project provides a **menu-driven interface** for various image transformations, filters, and enhancements.

---

## Table of Contents

- [Features](#features)  
- [Project Structure](#project-structure)  
- [Requirements](#requirements)  
- [Usage](#usage)  
- [Menu Example](#menu-example)  
- [Notes](#notes)  
- [Future Improvements](#future-improvements)  
- [Author](#author)  
- [License](#license)  

---

## Features

- **Basic Operations**: Load/Save image, Display, Change brightness, Contrast stretching, Adjust sharpness, Convert to binary  
- **Transformations**: Rotate (clockwise, anticlockwise, by angle), Flip (horizontal/vertical), Translate, Resize, Scale, Crop, Merge images (side-by-side / top-to-bottom)  
- **Filtering**: Mean filter, Median filter, Apply custom linear filters from file  
- **Enhancement & Analysis**: Laplacian enhancement, Sobel edge detection, Compute derivative  

---

## Project Structure

ImageProcessing/
│
├── main.cpp # Main program & menu-driven interface
├── Image.h / Image.cpp # Image class with processing functions
├── Menu.h / Menu.cpp # Menu class for interactive selection
├── MainMenu.txt # Menu items
├── filters/ # Optional folder for custom linear filters
└── screenshots/ # Screenshots for README or testing


---

## Requirements

- **C++ compiler** (C++11 or higher)   
- Input images in **PGM (P2 ASCII)** format  

---

Follow the Menu

Load a PGM image.
Apply transformations, filters, or enhancements.
Save the modified image.

---

Menu Example

MainMenu.txt example:

19
Load Image
Save Image
Change Brightness
Contrast Stretching
Adjust Sharpness
Convert to Binary
Resize Image
Rotate Image
Flip Image
Crop Image
Merge Images
Apply Mean/Median Filter
Apply Linear Filter
Enhance Image
Compute Derivative
Edge Detection
Translate Image
Scaling Image
Show Image
Exit

---

Notes

Works with grayscale images only.
Changes are not automatically saved; always use the save option.
Rotation by angle uses bilinear interpolation for smooth results.
Internal storage is vector<vector<int>> for ease of manipulation.

---

License

This project is open for educational use.
