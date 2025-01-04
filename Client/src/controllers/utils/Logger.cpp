#include "Logger.h"

// Initialize the static log level (default: DEBUG)
std::atomic<LogLevel> Logger::level = LogLevel::DEBUG;