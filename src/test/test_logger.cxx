// ----------------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2023 Carlos Carrasco
// ----------------------------------------------------------------------------
#include <cinttypes>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <streambuf>

#include <gtest/gtest.h>

#include <cxxlog/logger.h>
#include <cxxlog/transport.h>


// ----------------------------------------------------------------------------
// test_format
// ----------------------------------------------------------------------------
TEST (Logger, test_format) {
  std::stringstream ss {};

  const cxxlog::Logger<cxxlog::transport::OutputStream> logger { cxxlog::Severity::kVerbose };
  logger.transport (cxxlog::transport::OutputStream { ss });

  const int32_t i32 { 10 };
  const float f32 { 20.50501 };
  const double f64 { 30.068 };
  const uint64_t i64 { 40 };
  const char * const s { "hello" };
  const bool b { true };

  logger.verbose ("i32: {}, f32: {:.3f}, f64: {:.4f}, i64: {}, s: {}, b: {}", i32, f32, f64, i64, s, b);
  ASSERT_EQ (ss.str().substr(20), std::string ("V: i32: 10, f32: 20.505, f64: 30.0680, i64: 40, s: hello, b: true\n"));

  ss.str ("");
  logger.debug ("i32: {}, f32: {:.3f}, f64: {:.4f}, i64: {}, s: {}, b: {}", i32, f32, f64, i64, s, b);
  ASSERT_EQ (ss.str().substr(20), std::string ("D: i32: 10, f32: 20.505, f64: 30.0680, i64: 40, s: hello, b: true\n"));

  ss.str ("");
  logger.info ("i32: {}, f32: {:.3f}, f64: {:.4f}, i64: {}, s: {}, b: {}", i32, f32, f64, i64, s, b);
  ASSERT_EQ (ss.str().substr(20), std::string ("I: i32: 10, f32: 20.505, f64: 30.0680, i64: 40, s: hello, b: true\n"));

  ss.str ("");
  logger.warn ("i32: {}, f32: {:.3f}, f64: {:.4f}, i64: {}, s: {}, b: {}", i32, f32, f64, i64, s, b);
  ASSERT_EQ (ss.str().substr(20), std::string ("W: i32: 10, f32: 20.505, f64: 30.0680, i64: 40, s: hello, b: true\n"));

  ss.str ("");
  logger.error ("i32: {}, f32: {:.3f}, f64: {:.4f}, i64: {}, s: {}, b: {}", i32, f32, f64, i64, s, b);
  ASSERT_EQ (ss.str().substr(20), std::string ("E: i32: 10, f32: 20.505, f64: 30.0680, i64: 40, s: hello, b: true\n"));

  ss.str ("");
  logger.fatal ("i32: {}, f32: {:.3f}, f64: {:.4f}, i64: {}, s: {}, b: {}", i32, f32, f64, i64, s, b);
  ASSERT_EQ (ss.str().substr(20), std::string ("F: i32: 10, f32: 20.505, f64: 30.0680, i64: 40, s: hello, b: true\n"));
}

// ----------------------------------------------------------------------------
// test_severity
// ----------------------------------------------------------------------------
TEST (Logger, test_severity) {
  std::stringstream ss {};

  cxxlog::Logger<cxxlog::transport::OutputStream> logger { cxxlog::Severity::kFatal };
  logger.transport (cxxlog::transport::OutputStream { ss });

  // fatal
  logger.fatal ("test: {}", "logger");
  ASSERT_FALSE (ss.str().empty());
  ss.str ("");

  logger.error ("test: {}", "logger");
  logger.warn ("test: {}", "logger");
  logger.info ("test: {}", "logger");
  logger.debug ("test: {}", "logger");
  logger.verbose ("test: {}", "logger");
  ASSERT_TRUE (ss.str().empty());

  // error
  logger.setLevel (cxxlog::Severity::kError);

  logger.fatal ("test: {}", "logger");
  ASSERT_FALSE (ss.str().empty());
  ss.str ("");

  logger.error ("test: {}", "logger");
  ASSERT_FALSE (ss.str().empty());
  ss.str ("");

  logger.warn ("test: {}", "logger");
  logger.info ("test: {}", "logger");
  logger.debug ("test: {}", "logger");
  logger.verbose ("test: {}", "logger");
  ASSERT_TRUE (ss.str().empty());

  // warn
  logger.setLevel (cxxlog::Severity::kWarn);

  logger.fatal ("test: {}", "logger");
  ASSERT_FALSE (ss.str().empty());
  ss.str ("");

  logger.error ("test: {}", "logger");
  ASSERT_FALSE (ss.str().empty());
  ss.str ("");

  logger.warn ("test: {}", "logger");
  ASSERT_FALSE (ss.str().empty());
  ss.str ("");

  logger.info ("test: {}", "logger");
  logger.debug ("test: {}", "logger");
  logger.verbose ("test: {}", "logger");
  ASSERT_TRUE (ss.str().empty());

  // info
  logger.setLevel (cxxlog::Severity::kInfo);

  logger.fatal ("test: {}", "logger");
  ASSERT_FALSE (ss.str().empty());
  ss.str ("");

  logger.error ("test: {}", "logger");
  ASSERT_FALSE (ss.str().empty());
  ss.str ("");

  logger.warn ("test: {}", "logger");
  ASSERT_FALSE (ss.str().empty());
  ss.str ("");

  logger.info ("test: {}", "logger");
  ASSERT_FALSE (ss.str().empty());
  ss.str ("");

  logger.debug ("test: {}", "logger");
  ASSERT_TRUE (ss.str().empty());

  // debug
  logger.setLevel (cxxlog::Severity::kDebug);

  logger.fatal ("test: {}", "logger");
  ASSERT_FALSE (ss.str().empty());
  ss.str ("");

  logger.error ("test: {}", "logger");
  ASSERT_FALSE (ss.str().empty());
  ss.str ("");

  logger.warn ("test: {}", "logger");
  ASSERT_FALSE (ss.str().empty());
  ss.str ("");

  logger.info ("test: {}", "logger");
  ASSERT_FALSE (ss.str().empty());
  ss.str ("");

  logger.debug ("test: {}", "logger");
  ASSERT_FALSE (ss.str().empty());
  ss.str ("");

  logger.verbose ("test: {}", "logger");
  ASSERT_TRUE (ss.str().empty());

  // verbose
  logger.setLevel (cxxlog::Severity::kVerbose);

  logger.fatal ("test: {}", "logger");
  ASSERT_FALSE (ss.str().empty());
  ss.str ("");

  logger.error ("test: {}", "logger");
  ASSERT_FALSE (ss.str().empty());
  ss.str ("");

  logger.warn ("test: {}", "logger");
  ASSERT_FALSE (ss.str().empty());
  ss.str ("");

  logger.info ("test: {}", "logger");
  ASSERT_FALSE (ss.str().empty());
  ss.str ("");

  logger.debug ("test: {}", "logger");
  ASSERT_FALSE (ss.str().empty());
  ss.str ("");

  logger.verbose ("test: {}", "logger");
  ASSERT_FALSE (ss.str().empty());
  ss.str ("");

  // none
  logger.setLevel (cxxlog::Severity::kNone);

  logger.fatal ("test: {}", "logger");
  logger.error ("test: {}", "logger");
  logger.warn ("test: {}", "logger");
  logger.info ("test: {}", "logger");
  logger.debug ("test: {}", "logger");
  logger.verbose ("test: {}", "logger");
  ASSERT_TRUE (ss.str().empty());
}

// ----------------------------------------------------------------------------
// test_custom_transporter
// ----------------------------------------------------------------------------
TEST (Logger, test_custom_transporter) {
  class CustomTransport {
    public:
      CustomTransport (std::stringstream &ss): _ss { ss } {
        // empty
      }

      void log (const std::string &msg, cxxlog::Severity s, std::chrono::milliseconds ts) const {
        _ss.get() << ts.count() % 10000 << ": " << static_cast<int>(s) << " -> " << msg;
      }

    private:
      std::reference_wrapper<std::stringstream> _ss;
  };

  const cxxlog::Logger<CustomTransport> logger { cxxlog::Severity::kVerbose };

  const auto msecs {
    std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch())
  };

  std::stringstream ss;
  CustomTransport t { ss };
  logger.transport (t);

  logger.verbose ("hello verbose");
  ASSERT_EQ (ss.str(), cxxlog::fmtlib::format ("{}: 0 -> hello verbose", msecs.count() % 10000));
}

// ----------------------------------------------------------------------------
// test_multiple_transport
// ----------------------------------------------------------------------------
TEST (Logger, test_multiple_transport) {
  class CustomTransport {
    public:
      CustomTransport (std::stringstream &ss): _ss { ss } {
        // empty
      }

      void log (const std::string &msg, cxxlog::Severity s, std::chrono::milliseconds) const {
        _ss.get() << static_cast<int>(s) << " -> " << msg;
      }

    private:
      std::reference_wrapper<std::stringstream> _ss;
  };

  const cxxlog::Logger<CustomTransport, cxxlog::transport::OutputStream> logger { cxxlog::Severity::kVerbose };

  std::stringstream ss0;
  std::stringstream ss1;
  logger.transport (CustomTransport { ss0 });
  logger.transport (cxxlog::transport::OutputStream { ss1 });

  logger.debug ("hello debug");
  ASSERT_EQ (ss0.str(), "1 -> hello debug");
  ASSERT_NE (ss1.str().find (" D: hello debug"), std::string::npos);
}

// ----------------------------------------------------------------------------
// test_same_transport_multiple_times
// ----------------------------------------------------------------------------
TEST (Logger, test_same_transport_multiple_times) {
  const cxxlog::Logger<cxxlog::transport::OutputStream> logger { cxxlog::Severity::kVerbose };

  const auto tempFile { std::filesystem::temp_directory_path() / "test_same_transport_multiple_times.log" };
  if (std::filesystem::exists (tempFile))
    std::filesystem::remove (tempFile);

  ASSERT_FALSE (std::filesystem::exists (tempFile));

  std::ofstream fOut { tempFile, std::ios::out };
  ASSERT_TRUE (fOut.is_open());

  std::ostringstream sOut {};

  logger.transport (cxxlog::transport::OutputStream { sOut });
  logger.transport (cxxlog::transport::OutputStream { fOut });

  logger.fatal ("Hello World!");

  fOut.close();

  ASSERT_NE (sOut.str().find (" F: Hello World!\n"), std::string::npos);

  ASSERT_TRUE (std::filesystem::exists (tempFile));

  std::ifstream fIn { tempFile, std::ios::in };
  ASSERT_TRUE (fIn.is_open());

  std::string str ((std::istreambuf_iterator<char> { fIn }), std::istreambuf_iterator<char> {});

  ASSERT_NE (str.find (" F: Hello World!\n"), std::string::npos);
}
