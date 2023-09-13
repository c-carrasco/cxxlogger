// ----------------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2023 Carlos Carrasco
// ----------------------------------------------------------------------------
#include <cinttypes>
#include <sstream>

#include <gtest/gtest.h>

#include <cxxlog/logger.h>
#include <cxxlog/transport.h>


// ----------------------------------------------------------------------------
// OutputStreamTest class
// ----------------------------------------------------------------------------
class OutputStreamTest: public ::testing::Test {
  protected:
    std::stringstream stream;
    cxxlog::transport::OutputStream outputStream;

    OutputStreamTest(): outputStream { stream } {
      // empty
    }
};



// ----------------------------------------------------------------------------
// test_message_with_severity
// ----------------------------------------------------------------------------
TEST_F (OutputStreamTest, test_message_with_severity) {
  std::chrono::milliseconds ts { 1234567890 };
  outputStream.log ("Test message", cxxlog::Severity::kInfo, ts);
  const std::string logLine { stream.str() };

  EXPECT_EQ (stream.str(), "1970-01-15T06:56:07 I: Test message\n");
}

// ----------------------------------------------------------------------------
// test_multiple_messages
// ----------------------------------------------------------------------------
TEST_F(OutputStreamTest, test_multiple_messages) {
  std::chrono::milliseconds ts { 987654321000 };

  outputStream.log ("Message 1", cxxlog::Severity::kError, ts);
  outputStream.log ("Message 2", cxxlog::Severity::kWarn, ts);

  EXPECT_NE (stream.str().find ("2001-04-19T04:25:21 E: Message 1"), std::string::npos);
  EXPECT_NE (stream.str().find ("2001-04-19T04:25:21 W: Message 2"), std::string::npos);
}

// ----------------------------------------------------------------------------
// test_empty_message
// ----------------------------------------------------------------------------
TEST_F(OutputStreamTest, test_empty_message) {
  std::chrono::milliseconds ts { 1234567890 };

  outputStream.log ("", cxxlog::Severity::kDebug, ts);

  EXPECT_EQ (stream.str(), "1970-01-15T06:56:07 D: \n");
}