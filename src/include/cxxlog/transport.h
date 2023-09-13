// ----------------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2023 Carlos Carrasco
// ----------------------------------------------------------------------------
#ifndef __CXX_LOGGER_TRANSPORT_H__
#define __CXX_LOGGER_TRANSPORT_H__

#include <cinttypes>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <ostream>
#include <string>

#include <cxxlog/logger.h>


namespace cxxlog::transport {

/// @class OutputStream
/// @brief A class for logging messages to an output stream with timestamp and severity level.
///
/// This class provides a way to log messages to an output stream with a timestamp and a specified
/// severity level. The log messages are formatted in the following way:
/// "<timestamp> <severity>: <message>\n"
class OutputStream {
  public:
    /// @brief Constructor for the OutputStream class.
    /// @param out The output stream to which log messages will be written.
    OutputStream (std::ostream &out): _out { out } {
      // empty
    }

    /// @brief Log a message with a specified severity level and timestamp.
    /// @param msg The message to be logged.
    /// @param s The severity level of the message.
    /// @param ts Epoch time in milliseconds.
    ///
    /// This function logs a message with a specified severity level and includes a timestamp
    /// indicating when the message was logged. The message is written to the output stream
    /// provided in the constructor.
    ///
    /// @note The timestamp is UTC in the format "YYYY-MM-DDTHH:MM:SS".
    void log (const std::string &msg, Severity s, std::chrono::milliseconds ts) const {
      auto epochSecs { std::chrono::system_clock::to_time_t (std::chrono::time_point<std::chrono::system_clock> (ts)) };

      // format epochSecs as a date time to seconds resolution (e.g. 2016-08-30T08:18:51)
      struct tm buf {};
      _out.get()
        << std::put_time (gmtime_r (&epochSecs, &buf), "%FT%T")
        << " " << Logger<>::toCString (s)
        << ": " << msg << "\n";
    }

  private:
    std::reference_wrapper<std::ostream> _out;
};

}

#endif
