# Introduction
This is a lighweight and simple modern C++ logger header-only library.
It requires the [fmt](https://github.com/fmtlib/fmt) library for GCC < 13.x or Clang < 16.x.
For C++20 (or higher) with GCC >= 13.x or Clang >= 16 it uses `std::format` so there no external dependency.

# Requirements

- Conan 1.x
- CMake 3.20 or higer
- GCC, Clang or AppleClang
- Docker (optional)