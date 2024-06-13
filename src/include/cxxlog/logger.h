// ----------------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2023 Carlos Carrasco
// ----------------------------------------------------------------------------
#ifndef __CXX_LOGGER_H__
#define __CXX_LOGGER_H__
#include <array>
#include <chrono>
#include <cinttypes>
#include <list>
#include <string>
#include <utility>
#include <variant>

#ifdef CXXLOG_USE_FMT_LIBRARY
  #include <fmt/format.h>
#else
  #include <format>
#endif


namespace cxxlog {

/// @cond Doxygen_Suppress
#ifdef CXXLOG_USE_FMT_LIBRARY
  namespace fmtlib = fmt;
#else
  namespace fmtlib = std;
#endif
/// @endcond

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


/// @concept Loggable
///
/// @brief A concept that represents types capable of logging messages with a specified severity.
///
/// The Loggable concept defines the requirements for a type that can be used to log messages.
/// A Loggable type must provide a log method that takes a message (as a const std::string&)
/// and a severity level.
///
/// @tparam T The type to be checked for Loggable concept compliance.
///
/// @requirements
/// - T::log(const std::string &, Severity, std::chrono::milliseconds) const  Loggable types must have a log method with this signature.
template<typename T>
concept Loggable = requires (T l, const std::string &msg, Severity severity, std::chrono::milliseconds ts) {
  l.log (msg, severity, ts);
};

/// @brief A logger class for handling and formatting log messages with different severity levels.
///
/// The Logger class provides functionality to log messages of various severity levels, such as
/// verbose, debug, info, warning, error, and fatal. It supports multiple logger transports for
/// handling the log output, and you can set the severity level threshold to filter messages.
///
/// @tparam Ts Variadic template parameters representing the Loggable transport classes.
template<Loggable... Ts>
class Logger {
  public:
    /// @brief Constructor for the Logger class.
    /// @param severity The severity level threshold for logging.
    Logger (Severity severity = Severity::kInfo) noexcept: _severity { severity } {
      // empty
    }

    /// @brief Add a new logger transport.
    ///
    ///  A transport is essentially a class which will handle what to do with the logs,
    /// such as printing to the console, sending to a server, etc.
    ///
    /// @tparam T The type of the transport class.
    /// @param t An instance of the transport class to be added.
    template<Loggable T>
    inline void transport (T &&t) const {
      _transport.emplace_back (std::forward<T> (t));
    }

    /// @brief Logs a message.
    ///
    /// This method is used to log a message for the specified verbosity level. The message
    /// will be output only if the logger is enabled for the specified severity level.
    ///
    /// @tparam Args Variadic template parameter for the arguments to format the log message.
    /// @param s The severity level of the message.
    /// @param fmt The format string for the log message.
    /// @param args The arguments to be inserted into the format string.
    template<typename... Args>
    inline void log (Severity s, fmtlib::format_string<Args...> fmt, Args && ... args) const {
      if (isEnabled (s)) {
          const auto msg { fmtlib::format (fmt, std::forward<Args>(args)...) };
          const auto ts { std::chrono::duration_cast<std::chrono::milliseconds> (
            std::chrono::system_clock::now().time_since_epoch()
          ) };

          for (const auto &t: _transport)
            std::visit ([ &msg, s, ts ] (const auto &t) { t.log (msg, s, ts); }, t);
      }
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
      log (Severity::kVerbose, fmt, std::forward<Args>(args)...);
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
      log (Severity::kDebug, fmt, std::forward<Args>(args)...);
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
      log (Severity::kInfo, fmt, std::forward<Args>(args)...);
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
      log (Severity::kWarn, fmt, std::forward<Args>(args)...);
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
      log (Severity::kError, fmt, std::forward<Args>(args)...);
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
      log (Severity::kFatal, fmt, std::forward<Args>(args)...);
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

    /// @brief Converts a Severity enum value to its corresponding string representation.
    /// @param s The severity level to convert.
    /// @return The string representation of the severity level.
    static inline constexpr const char * toCString (Severity s) { return kStrLevels[static_cast<int_fast8_t> (s)]; }

  private:
    mutable std::list<std::variant<Ts...>> _transport {};
    Severity _severity;

    static constexpr std::array<const char *, 6> kStrLevels { "V", "D", "I", "W", "E", "F" };
};

}

#endif
