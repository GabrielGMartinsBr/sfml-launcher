#pragma once

#include <cstdarg>
#include <string>

struct StringUtils {
  static std::string format(const char* format, ...)
  {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    std::vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    return std::string(buffer);
  }
};