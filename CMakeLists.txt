cmake_minimum_required (VERSION 3.20)

project (
  "C++ Logger"
  DESCRIPTION "A simple logger library for modern C++"
  HOMEPAGE_URL "https://github.com/c-carrasco/loggercpp"
  LANGUAGES CXX
)

include (cmake/configure_compiler.cmake)
include (cmake/configure_version.cmake)
include (cmake/conan.cmake)
include (cmake/doxygen.cmake)

if (NOT SUPPORT_STD_FORMAT OR CMAKE_CXX_STANDARD LESS_EQUAL 17)
  add_compile_definitions (CXXLOG_USE_FMT_LIBRARY)
endif()

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set (CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type options are: Debug, Release")

list (APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package (fmt REQUIRED)
find_package (GTest REQUIRED)

find_program (CCACHE_PROGRAM ccache)

if (CCACHE_PROGRAM)
  message (STATUS "Enabling ccache")
  set_property (GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

enable_testing()

include (CTest)

add_subdirectory (src)