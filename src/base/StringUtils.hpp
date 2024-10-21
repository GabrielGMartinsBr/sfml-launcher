#pragma once

#include <SFML/System/String.hpp>
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

  static sf::String fromUtf8(const std::string& in)
  {
    std::basic_string<sf::Uint32> tmp;
    sf::Utf8::toUtf32(in.begin(), in.end(), back_inserter(tmp));
    return sf::String(tmp);
  }

  static std::string toUtf8(const sf::String& str)
  {
    std::string out;
    sf::Utf32::toUtf8(str.begin(), str.end(), back_inserter(out));
    return out;
  }
};