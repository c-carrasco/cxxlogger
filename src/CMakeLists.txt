add_library (cxxlogger INTERFACE)

target_include_directories (cxxlogger INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries (cxxlogger INTERFACE fmt::fmt)

if (CXXLOG_BUILD_TESTS)
  add_subdirectory (test)
endif()