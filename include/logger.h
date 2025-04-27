//
// Created by Seongyun Jang on 3/31/25.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <mutex>

using namespace std;

class Logger {
public:
  // Logging by levels
  enum class LogLevel { INFO, WARNING, ERROR, DEBUG };
  static void LOG_INFO(const string &message);
  static void LOG_WARNING(const string &message);
  static void LOG_ERROR(const string &message);
  static void LOG_DEBUG(const string &message);

  // Get the singleton instance
  static Logger *getInstance();
  static mutex instanceMutex;

  // Delete copy constructor and assignment operator
  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;
  Logger(Logger &&) = delete;
  Logger &operator=(Logger &&) = delete;

private:
  // Private constructor
  Logger();
  // Private destructor
  ~Logger();
  // Logging method
  void log(const string &message, LogLevel level);

  // Singleton instance
  static Logger *instance;
  // Log file stream
  ofstream log_file;
  // Predefined log level strings
  static constexpr string log_level_strings[4] = {"[INFO]", "[WARNING]",
                                                  "[ERROR]", "[DEBUG]"};
  // Predefined log level colors
  static constexpr string log_level_colors[4] = {
      "\033[0;32m", // Green
      "\033[0;33m", // Yellow
      "\033[0;31m", // Red
      "\033[0;34m"  // Blue
  };
  // Predefined log level reset color
  static constexpr string log_level_reset = "\033[0m";
  // Predefined log format
  static constexpr auto const *log_format = "{0} {1}{2}{3} {4}\n";
};

#endif // LOGGER_H
