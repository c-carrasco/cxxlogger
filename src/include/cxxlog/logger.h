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

//! @cond Doxygen_Suppress
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
//! @endcond

/// @class Logger logger.h hacommon/logger.h
/// @brief Logger Class
///
/// This class makes it possible to use log file in multi-threaded applications.
/// The information added at the begin of each line can be customized:
///   void MyCustomInfo (std::ostream &out, Logger::Level l) {
///     out << "My custom header " << static_cast<int>(l) << " - ";
///   }
///   ...
///   int main () {
///     Logger log (std::cout, Logger::kDebug, MyCustomInfo);
///     ...
///     log.info ("hello world");
///     log.error ("unexpected error");
///     ...
///   }
///
///   ---- stdout ----
///   My Custom Header 1 - hello world
///   My Custom Header 3 - unexpected error
///
///  By default Logger adds the following information:
///   MM/DD/YYYY HH:MM:SS LEVEL -
///  The class also allow to specify what kind of info is being added to the log
///  file and what kind of info must be added.
class Logger {
  public:
    enum class Severity: std::int_fast8_t {
      kVerbose = 0x00,
      kDebug = 0x01,
      kInfo = 0x02,
      kWarn = 0x03,
      kError = 0x04,
      kFatal = 0x05,
      kNone = 0x7F
    };

    using FormatHeader = std::function<void (std::ostream &out, Logger::Severity)>;

    /// @brief Constructor
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

    /// @brief Prints messages with the level Severity::kVerbose
    template<typename... Args>
    inline void verbose (fmtlib::format_string<Args...> fmt, Args && ... args) const {
      if (isEnabled (Severity::kVerbose)) {
        _fmtHeader (_out, Severity::kVerbose);
        fmtlib::print (_out, fmt, std::forward<Args>(args)...);
      }
    }

    /// @brief Prints messages with the level Severity::kDebug
    template<typename... Args>
    inline void debug (fmtlib::format_string<Args...> fmt, Args && ... args) const {
      if (isEnabled (Severity::kDebug)) {
        _fmtHeader (_out, Severity::kDebug);
        fmtlib::print (_out, fmt, std::forward<Args>(args)...);
      }
    }

    /// @brief Prints messages with the level Level.kInfo
    template<typename... Args>
    inline void info (fmtlib::format_string<Args...> fmt, Args && ... args) const {
      if (isEnabled (Severity::kInfo)) {
        _fmtHeader (_out, Severity::kInfo);
        fmtlib::print (_out, fmt, std::forward<Args>(args)...);
      }
    }

    /// @brief Prints messages with the level Level.kWarn
    template<typename... Args>
    inline void warn (fmtlib::format_string<Args...> fmt, Args && ... args) const {
      if (isEnabled (Severity::kWarn)) {
        _fmtHeader (_out, Severity::kWarn);
        fmtlib::print (_out, fmt, std::forward<Args>(args)...);
      }
    }

    /// @brief Prints messages with the level Level.kError
    template<typename... Args>
    inline void error (fmtlib::format_string<Args...> fmt, Args && ... args) const {
      if (isEnabled (Severity::kError)) {
        _fmtHeader (_out, Severity::kError);
        fmtlib::print (_out, fmt, std::forward<Args>(args)...);
      }
    }

    /// @brief Prints messages with the level Level.kFatal
    template<typename... Args>
    inline void fatal (fmtlib::format_string<Args...> fmt, Args && ... args) const {
      if (isEnabled (Severity::kFatal)) {
        _fmtHeader (_out, Severity::kFatal);
        fmtlib::print (_out, fmt, std::forward<Args>(args)...);
      }
    }

    /// @brief Sets the threshold for this logger to level `l`. Logging messages which are less severe than level will be ignored
    inline Severity setLevel (Severity s) noexcept { return std::exchange (_severity, s); }

    /// @brief Returns the current level.
    inline Severity getLevel () const noexcept { return _severity; }

    /// @brief Returns if the current logger is enabled for the specified level.
    inline bool isEnabled (Severity s) const noexcept { return _severity <= s; }

  private:
    std::ostream &_out;
    Severity _severity;
    FormatHeader _fmtHeader;


    static constexpr std::array<const char *, 6> kStrLevels { "V", "D", "I", "W", "E", "F" };

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
