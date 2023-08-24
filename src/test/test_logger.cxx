// ----------------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2023 Carlos Carrasco
// ----------------------------------------------------------------------------
#include <cinttypes>
#include <sstream>

#include <gtest/gtest.h>

#include <cxxlog/logger.h>


// ----------------------------------------------------------------------------
// level2str
// ----------------------------------------------------------------------------
static const char * level2str (cxxlog::Logger::Severity l) {
  return (l == cxxlog::Logger::Severity::kVerbose)? "V" :
    (l == cxxlog::Logger::Severity::kDebug)? "D" :
    (l == cxxlog::Logger::Severity::kInfo)? "I" :
    (l == cxxlog::Logger::Severity::kWarn)? "W" :
    (l == cxxlog::Logger::Severity::kError)? "E" :
    (l == cxxlog::Logger::Severity::kFatal)? "F" : "Unknown";
}


// ----------------------------------------------------------------------------
// test_printf_format
// ----------------------------------------------------------------------------
TEST (Logger, test_printf_format) {
  std::stringstream ss;

  const cxxlog::Logger logger {
    ss,
    cxxlog::Logger::Severity::kVerbose,
    [] (std::ostream &out, cxxlog::Logger::Severity l) {
      out << level2str (l) << " -> ";
    }
  };

  const int32_t i32 { 10 };
  const float f32 { 20.50501 };
  const double f64 { 30.068 };
  const uint64_t i64 { 40 };
  const char * const s { "hello" };
  const bool b { true };

  logger.verbose ("i32: {}, f32: {:.3f}, f64: {:.4f}, i64: {}, s: {}, b: {}", i32, f32, f64, i64, s, b);
  ASSERT_EQ (ss.str(), std::string ("V -> i32: 10, f32: 20.505, f64: 30.0680, i64: 40, s: hello, b: true"));

  ss.str ("");
  logger.debug ("i32: {}, f32: {:.3f}, f64: {:.4f}, i64: {}, s: {}, b: {}", i32, f32, f64, i64, s, b);
  ASSERT_EQ (ss.str(), std::string ("D -> i32: 10, f32: 20.505, f64: 30.0680, i64: 40, s: hello, b: true"));

  ss.str ("");
  logger.info ("i32: {}, f32: {:.3f}, f64: {:.4f}, i64: {}, s: {}, b: {}", i32, f32, f64, i64, s, b);
  ASSERT_EQ (ss.str(), std::string ("I -> i32: 10, f32: 20.505, f64: 30.0680, i64: 40, s: hello, b: true"));

  ss.str ("");
  logger.warn ("i32: {}, f32: {:.3f}, f64: {:.4f}, i64: {}, s: {}, b: {}", i32, f32, f64, i64, s, b);
  ASSERT_EQ (ss.str(), std::string ("W -> i32: 10, f32: 20.505, f64: 30.0680, i64: 40, s: hello, b: true"));

  ss.str ("");
  logger.error ("i32: {}, f32: {:.3f}, f64: {:.4f}, i64: {}, s: {}, b: {}", i32, f32, f64, i64, s, b);
  ASSERT_EQ (ss.str(), std::string ("E -> i32: 10, f32: 20.505, f64: 30.0680, i64: 40, s: hello, b: true"));

  ss.str ("");
  logger.fatal ("i32: {}, f32: {:.3f}, f64: {:.4f}, i64: {}, s: {}, b: {}", i32, f32, f64, i64, s, b);
  ASSERT_EQ (ss.str(), std::string ("F -> i32: 10, f32: 20.505, f64: 30.0680, i64: 40, s: hello, b: true"));
}

// ----------------------------------------------------------------------------
// test_level
// ----------------------------------------------------------------------------
TEST (Logger, test_level) {
  std::stringstream ss;
  cxxlog::Logger logger { ss, cxxlog::Logger::Severity::kFatal };

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
  logger.setLevel (cxxlog::Logger::Severity::kError);

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
  logger.setLevel (cxxlog::Logger::Severity::kWarn);

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
  logger.setLevel (cxxlog::Logger::Severity::kInfo);

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
  logger.setLevel (cxxlog::Logger::Severity::kDebug);

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
  logger.setLevel (cxxlog::Logger::Severity::kVerbose);

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
  logger.setLevel (cxxlog::Logger::Severity::kNone);

  logger.fatal ("test: {}", "logger");
  logger.error ("test: {}", "logger");
  logger.warn ("test: {}", "logger");
  logger.info ("test: {}", "logger");
  logger.debug ("test: {}", "logger");
  logger.verbose ("test: {}", "logger");
  ASSERT_TRUE (ss.str().empty());
}
