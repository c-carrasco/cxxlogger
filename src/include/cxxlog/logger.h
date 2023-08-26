// ----------------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2023 Carlos Carrasco
// ----------------------------------------------------------------------------
#ifndef __CXX_LOGGER_H__
#define __CXX_LOGGER_H__
#include <array>
#include <functional>
#include <ostream>

#ifdef CXXLOG_USE_FMT_LIBRARY
  #include <fmt/ostream.h>
  #include <fmt/format.h>
  #include <fmt/core.h>
  #include <fmt/chrono.h>
#else
  #include <format>
#endif


namespace cxxlog {

/// @cond Doxygen_Suppress
#ifdef CXXLOG_USE_FMT_LIBRARY
  namespace fmtlib = fmt;
#else
  namespace fmtlib {

  template<typename... Args>
  inline void print (std::ostream& os, std::format_string<Args...> fmt, Args&&... args) {
    os << std::format (fmt, std::forward<Args>(args)...);
  }

  using namespace std;

  }
#endif
/// @endcond

/// @class Logger logger.h cxxlog/logger.h
/// @brief A logging utility for outputting messages with different severity levels.
///
/// The information added at the begin of each line can be customized:
/// @code{.cpp}
///   static void myCustomPrefix (std::ostream &out, cxxlog::Logger::Severity s) {
///     out << "My custom header " << static_cast<int>(s) << " - ";
///   }
///   ...
///   int main () {
///     Logger log (std::cout, Logger::kDebug, myCustomPrefix);
///     ...
///     log.info ("hello world");
///     log.error ("unexpected error");
///     ...
///   }
/// @endcode
/// @code{.sh}
///   ---- stdout ----
///   My Custom Header 1 - hello world
///   My Custom Header 3 - unexpected error
/// @endcode
/// By default Logger adds the following information:
/// @code{.sh}
///   MM/DD/YYYY HH:MM:SS LEVEL -
/// @endcode
///  The class also allow to specify what kind of info is being added to the log
///  file and what kind of info must be added.
class Logger {
  public:
    /// @enum Severity
    /// @brief Enumeration representing different severity levels for log messages
    enum class Severity: std::int_fast8_t {
      kVerbose = 0x00, ///< Verbose level.
      kDebug = 0x01,   ///< Debug level.
      kInfo = 0x02,    ///< Information level.
      kWarn = 0x03,    ///< Warning level.
      kError = 0x04,   ///< Error level.
      kFatal = 0x05,   ///< Fatal error level.
      kNone = 0x7F     ///< No logging.
    };

    using FormatHeader = std::function<void (std::ostream &out, Logger::Severity)>;

    /// @brief Constructor for the Logger class.
    /// @param out The output stream to which log messages will be written.
    /// @param severity The severity level threshold for logging.
    /// @param format The formatting function for log message headers.
    Logger (
      std::ostream &out,
      Severity severity = Severity::kInfo,
      FormatHeader format = _formatHeader
    ) noexcept:
      _out { out },
      _severity { severity },
      _fmtHeader { std::move (format) }
    {
      // empty
    }

    /// @brief Logs a verbose-level message.
    ///
    /// This method is used to log a verbose-level message. The message will be output only
    /// if the logger is enabled for the verbose severity level.
    ///
    /// @tparam Args Variadic template parameter for the arguments to format the log message.
    /// @param fmt The format string for the log message.
    /// @param args The arguments to be inserted into the format string.
    template<typename... Args>
    inline void verbose (fmtlib::format_string<Args...> fmt, Args && ... args) const {
      if (isEnabled (Severity::kVerbose)) {
        _fmtHeader (_out, Severity::kVerbose);
        fmtlib::print (_out, fmt, std::forward<Args>(args)...);
      }
    }

    /// @brief Logs a debug-level message.
    ///
    /// This method is used to log a debug-level message. The message will be output only
    /// if the logger is enabled for the debug severity level.
    ///
    /// @tparam Args Variadic template parameter for the arguments to format the log message.
    /// @param fmt The format string for the log message.
    /// @param args The arguments to be inserted into the format string.
    template<typename... Args>
    inline void debug (fmtlib::format_string<Args...> fmt, Args && ... args) const {
      if (isEnabled (Severity::kDebug)) {
        _fmtHeader (_out, Severity::kDebug);
        fmtlib::print (_out, fmt, std::forward<Args>(args)...);
      }
    }

    /// @brief Logs an info-level message.
    ///
    /// This method is used to log an info-level message. The message will be output only
    /// if the logger is enabled for the info severity level.
    ///
    /// @tparam Args Variadic template parameter for the arguments to format the log message.
    /// @param fmt The format string for the log message.
    /// @param args The arguments to be inserted into the format string.
    template<typename... Args>
    inline void info (fmtlib::format_string<Args...> fmt, Args && ... args) const {
      if (isEnabled (Severity::kInfo)) {
        _fmtHeader (_out, Severity::kInfo);
        fmtlib::print (_out, fmt, std::forward<Args>(args)...);
      }
    }

    /// @brief Logs a warning-level message.
    ///
    /// This method is used to log a warning-level message. The message will be output only
    /// if the logger is enabled for the warning severity level.
    ///
    /// @tparam Args Variadic template parameter for the arguments to format the log message.
    /// @param fmt The format string for the log message.
    /// @param args The arguments to be inserted into the format string.
    template<typename... Args>
    inline void warn (fmtlib::format_string<Args...> fmt, Args && ... args) const {
      if (isEnabled (Severity::kWarn)) {
        _fmtHeader (_out, Severity::kWarn);
        fmtlib::print (_out, fmt, std::forward<Args>(args)...);
      }
    }

    /// @brief Logs an error-level message.
    ///
    /// This method is used to log an error-level message. The message will be output only
    /// if the logger is enabled for the error severity level.
    ///
    /// @tparam Args Variadic template parameter for the arguments to format the log message.
    /// @param fmt The format string for the log message.
    /// @param args The arguments to be inserted into the format string.
    template<typename... Args>
    inline void error (fmtlib::format_string<Args...> fmt, Args && ... args) const {
      if (isEnabled (Severity::kError)) {
        _fmtHeader (_out, Severity::kError);
        fmtlib::print (_out, fmt, std::forward<Args>(args)...);
      }
    }

    /// @brief Logs a fatal-level message.
    ///
    /// This method is used to log a fatal-level message. The message will be output only
    /// if the logger is enabled for the fatal severity level.
    ///
    /// @tparam Args Variadic template parameter for the arguments to format the log message.
    /// @param fmt The format string for the log message.
    /// @param args The arguments to be inserted into the format string.
    template<typename... Args>
    inline void fatal (fmtlib::format_string<Args...> fmt, Args && ... args) const {
      if (isEnabled (Severity::kFatal)) {
        _fmtHeader (_out, Severity::kFatal);
        fmtlib::print (_out, fmt, std::forward<Args>(args)...);
      }
    }

    /// @brief Sets the severity threshold for the logger.
    ///
    /// Logging messages which are less severe than the specified level will be ignored.
    ///
    /// @param s The new severity level threshold.
    /// @return The previous severity level threshold.
    inline Severity setLevel (Severity s) noexcept { return std::exchange (_severity, s); }

    /// @brief Gets the current severity level threshold of the logger.
    /// @return The current severity level threshold.
    inline Severity getLevel () const noexcept { return _severity; }

    /// @brief Checks if the logger is enabled for the specified severity level.
    /// @param s The severity level to check.
    /// @return `true` if the logger is enabled for the specified level, otherwise `false`.
    inline bool isEnabled (Severity s) const noexcept { return _severity <= s; }

  private:
    std::ostream &_out;
    Severity _severity;
    FormatHeader _fmtHeader;


    static constexpr std::array<const char *, 6> kStrLevels { "V", "D", "I", "W", "E", "F" };

    /// @brief The dedault method to format and output the log message header.
    /// @param out The output stream.
    /// @param s The severity level of the log message.
    static void _formatHeader (std::ostream &out, Severity s) {
      const std::time_t now { std::time (nullptr) };
      const auto *t { std::localtime (&now) };
      out << fmtlib::format (
        "{:04d}/{:02d}/{:02d} {:02d}:{:02d}:{:02d} {:<1}: ",
        t->tm_year, t->tm_mon + 1, t->tm_mday,
        t->tm_hour, t->tm_min, t->tm_sec,
        kStrLevels[static_cast<int_fast8_t> (s)]
      );
    }
};

}

#endif
