#pragma once

#include <boost/locale.hpp>
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

  static std::size_t length(const std::string& str)
  {
    using namespace boost::locale::conv;
    std::wstring wstr = utf_to_utf<wchar_t>(str);
    return wstr.size();
  }

  static std::string lengthStr(const std::string& str)
  {
    using namespace boost::locale::conv;
    std::wstring wstr = utf_to_utf<wchar_t>(str);
    return std::to_string(wstr.size());
  }
};