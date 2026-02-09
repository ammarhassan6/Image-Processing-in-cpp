# Image Processing Project in C++

A **console-based C++ image processing application** that allows loading, manipulating, and saving grayscale images (PGM format). The project provides a **menu-driven interface** for various image transformations, filters, and enhancements.

---

## Table of Contents

- [Features](#features)  
- [Notes](#notes)  

---

## Features

- **Basic Operations**: Load/Save image, Display, Change brightness, Contrast stretching, Adjust sharpness, Convert to binary  
- **Transformations**: Rotate (clockwise, anticlockwise, by angle), Flip (horizontal/vertical), Translate, Resize, Scale, Crop, Merge images (side-by-side / top-to-bottom)  
- **Filtering**: Mean filter, Median filter, Apply custom linear filters from file  
- **Enhancement & Analysis**: Laplacian enhancement, Sobel edge detection, Compute derivative  

---

Follow the Menu

Load a PGM image.
Apply transformations, filters, or enhancements.
Save the modified image.

---

Notes

Works with grayscale images only.
Changes are not automatically saved; always use the save option.
Rotation by angle uses bilinear interpolation for smooth results.
Internal storage is vector<vector<int>> for ease of manipulation.

---

License

This project is open for educational use.
