CXX Logger
==========

[![Linux](https://github.com/c-carrasco/cxxlogger/actions/workflows/main.yml/badge.svg)](https://github.com/c-carrasco/cxxlogger/actions/workflows/main.yml)

# Overview

Welcome to the CXX Logger, a lightweight, header-only library designed for easy logging in C++20 or newer standards.
This library provides a hassle-free experience and is designed to be a simple and universal logging library with support for multiple transports for incorporating logging capabilities into your projects. A transport is essentially a storage device for your logs.
As a workaround for older compilers (GCC < 13, Clang < 16 and AppleClang < 15) the library support [fmt library](https://github.com/fmtlib/fmt).

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

Examples:

```
# Compile code in release mode
./build.sh clean release

# Build code in debug mode using Docker image with Clang 16, Address Sanitizer enabled, and run unit tests
./build.sh docker=clang16 debug test asan=on

# Start docker dev environment with gcc13
./build.sh docker=gcc13
```

# Quick Start Guide

### 1. Include the `logger.h` header.

```CPP
#include <cxxlog/logger.h>
```

**Note:** All functions, classes, structures, enums, ...  reside within the `cxxlog` namespace.

### 2. Initialize your Logger with built-In transports

```CPP
  const cxxlog::Logger<cxxlog::transport::OutputStream> logger { cxxlog::Severity::kDebug };
  logger.transport (cxxlog::transport::OutputStream { std::cout });
```

### or use your custom transports

```CPP
  struct MyCustomTransport {
    void log (const std::string &msg, cxxlog::Severity severity, std::chrono::milliseconds ts) const {
      // Your custom logging logic here
      // ...
    }
  };

  const cxxlog::Logger<
    cxxlog::transport::OutputStream,
    MyCustomTransport
  > logger { cxxlog::Severity::kDebug };

  logger.transport (cxxlog::transport::OutputStream { std::cout });
  logger.transport (MyCustomTransport {});
```

### 3. Start logging messages

```CPP
  logger.info ("Hello world!");
  ...
  logger.debug ("number of message: {}/{}", count, total);
  ...
```

# Creating custom transports.

The library allows you to create your own transport classes to extend and customize the logging capabilities.

A custom transport is a class with a `log` method, having the following signature:

```CPP
  void log (const std::string &msg, cxxlog::Severity severity, std::chrono::milliseconds ts) const;
```

Inside the method, you can define you own logging logic:

```CPP
  class AndroidLog {
    public:
      AndroidLog (std::string tag): _tag { std::move (tag) } {
        // empty
      }

      void log (const std::string &msg, cxxlog::Severity severity, std::chrono::milliseconds) const {
        __android_log_write (kMap[static_cast<int>(severity)], _tag.c_str(), msg.c_str());
      }

    private:
      std::string _tag;

      static constexpr std::array<int, 6> kMap {
        ANDROID_LOG_VERBOSE,
        ANDROID_LOG_DEBUG,
        ANDROID_LOG_INFO,
        ANDROID_LOG_WARN,
        ANDROID_LOG_ERROR,
        ANDROID_LOG_FATAL
      };
  };

```

# Built-in transports

There are several core transports included in `cxxlog::transport` namespace.

- *cxxlog::transport::OutputStream*: Stores log messages into the specified [std::ostream](https://en.cppreference.com/w/cpp/io/basic_ostream) object (e.g., std::cout or std::ofstream).

# Installation

To use the library, follow these steps:

- 1. Copy the `src/include/cxxlog` folder into your project.
- 2. If you want/need to use the `fmt library`, add the `CXXLOG_USE_FMT_LIBRARY` preprocessor definition to your compilation options. Don't forget to link against _fmt_ library.
