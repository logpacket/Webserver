//
// Created by Seongyun Jang on 3/31/25.
//

#include "logger.h"

#include <iostream>
#include <format>
#include <string>
#include <sstream>
#include <mutex>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <filesystem>
#include <cerrno>

mutex Logger::instanceMutex;

Logger *Logger::getInstance() {
    static Logger instance;
    return &instance;
}

Logger::Logger() {
    // Open log file
    this->log_file.open("webserver.log", ios::app);
    if (!this->log_file.is_open()) {
        cerr << "Error opening log file: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
}

Logger::~Logger() {
    if (this->log_file.is_open()) {
        this->log_file.close();
    }
}

void Logger::log(const string &message, const LogLevel level) {
    // Get current time
    const auto now = chrono::system_clock::now();
    const auto time_t_now = chrono::system_clock::to_time_t(now);
    tm local_time{};
    localtime_r(&time_t_now, &local_time);

    // Format time
    stringstream time_stream;
    time_stream << put_time(&local_time, "%Y-%m-%d %H:%M:%S");

    // Log message
    string level_str = log_level_strings[static_cast<int>(level)];

    // Construct the log message
    string log_message = format(
        log_format,
        time_stream.str(),
        log_level_colors[static_cast<int>(level)],
        level_str,
        log_level_reset,
        message
    );

    // Lock the mutex to ensure thread safety
    lock_guard lock(instanceMutex);

    // Write to log file and console
    this->log_file << log_message;
    level == LogLevel::ERROR ? cerr << log_message : cout << log_message;
}

void Logger::LOG_INFO(const string &message) {
    getInstance()->log(message, LogLevel::INFO);
}

void Logger::LOG_WARNING(const string &message) {
    getInstance()->log(message, LogLevel::WARNING);
}

void Logger::LOG_ERROR(const string &message) {
    getInstance()->log(message, LogLevel::ERROR);
}

void Logger::LOG_DEBUG(const string &message) {
    getInstance()->log(message, LogLevel::DEBUG);
}
