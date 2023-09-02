CXX Logger
==========

[![Linux](https://github.com/c-carrasco/cxxlogger/actions/workflows/main.yml/badge.svg)](https://github.com/c-carrasco/cxxlogger/actions/workflows/main.yml)

# Overview

Welcome to the CXX Logger, a lightweight, header-only library designed for easy logging in C++17 or newer standards.
This library provides a hassle-free experience for incorporating logging capabilities into your projects. It utilizes the [fmt library](https://github.com/fmtlib/fmt) for C++17, but seamlessly transitions to using std::format for C++20 (or newer) with GCC versions 13.x or higher, as well as Clang versions 16 or higher, eliminating external dependencies.

# Prerequisites

Before diving into CXX Logger, make sure you have the following tools and dependencies set up:

- Conan 1.x
- CMake 3.20 or higer
- GCC, Clang or AppleClang
- GNU Make or ninja
- Docker (optional)
- Doxygen (for generating documentation)

# Quick Guide to Getting Started

## Achieve Reproducible Builds with Docker

For a streamlined development environment, Docker is your friend. It's highly recommended to utilize Docker for building your code.

## The Building Script

Located in the root folder of the project, the build.sh file acts as the catalyst for your building endeavors.

**Usage Guide**
---------------
* **_release_**
Construct the code in release mode.
* **_debug_**
Construct the code in debug mode (selected if no mode is specified).
* **_clean_**
Wipe out the _build_ directory along with its contents.
* **_verbose_**
Force GNU make to show the commands.
* **_ninja_**
Opt for `ninja` over GNU Make for code compilation.
* **_tests_**
Execute tests post compilation.
* **_asan=on_**
Enable the [Address Sanitizer](#section5_1)
* **_ubsan=on_**
Enable the [Undefined Behavior Sanitizer](#section5_2)
* **_docker[=compiler]_**
Use docker for local development.
  Available options:
    - `gcc13`: Use GCC 13 as default compiler (selected if no compiler is specified).
    - `clang16`: Use Clang 16 as default compiler.
* **_cxx17_**
Set the C++ standard to C++17 (C++20 by default)

Examples:

```
# Compile code in release mode
./build.sh clean release

# Build code in debug mode using Docker image with Clang 16, C++17, Address Sanitizer enabled, and run unit tests
./build.sh docker=clang16 debug cxx17 test asan=on

# Start docker dev environment with gcc13
./build.sh docker=gcc13
```

# Getting Started in a Flash

1. Include the `logger.h` header.

```CPP
#include <cxxlog/logger.h>
```

**Note:** All functions, classes, structures, enums, ...  reside within the `cxxlog` namespace.

2. Craft your Logger instance

```CPP
  const cxxlog::Logger logger { std::cout, cxxlog::Logger::kDebug };
```

3. and just log messages.

```CPP
  logger.info ("Hello world!\n");
  ...
  logger.debug ("number of message: {}/{}\n", count, total);
  ...
```

# Customizing Your Log Header

By default `cxxlog::Logger` prefixes each log message with the local timestamp and its corresponding severity level:

```
2023/08/26 18:45:09 W: ...
```

However, you can exercise full control over the formatting by crafting your custom header during the logger instantiation:

```CPP
  const cxxlog::Logger logger {
    std::cout,
    cxxlog::Logger::kDebug,
    [] (std::ostream &out, Severity s) {
      out << "[FOO: " << static_cast<int> (s) << "] ";
    }
  };

  logger.info ("Hello, world!\n"); // outputs: [FOO:1] Hello, world!
```

# Installation

Copy the `src/include/cxxlog` folder to your project and if you want/need to use the `fmt library` add the `CXXLOG_USE_FMT_LIBRARY` preprocessor definition to the compilation options (and don't forget to link against _fmt_).
