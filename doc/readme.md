CXX Logger Library
==================

**Welcome to the CXX Logger Library**, an intuitive header-only solution tailored for seamless logging in C++17 and newer standards. This library streamlines the process of incorporating robust logging capabilities into your projects. Leveraging the [fmt library](https://github.com/fmtlib/fmt) for C++17, the library gracefully transitions to the std::format feature available in C++20 (or newer). Notably, this transition is smooth with GCC versions 13.x or later, as well as Clang versions 16 and above, effectively eliminating external dependencies.

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

# {fmt} library

If you need/want to use [{fmt}](https://github.com/fmtlib/fmt) library, you have to add the `CXXLOG_USE_FMT_LIBRARY` preprocessor definition to the compilation options.

# Installation

Copy the `src/include/cxxlog` folder to your project.