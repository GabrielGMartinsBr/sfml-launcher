#pragma once

#include <regex>

class BacktraceUtils {

 public:
  static long pickLineNumber(const char* message)
  {
    std::regex pattern(R"(^[^:]+:(\d+))");
    std::cmatch match;

    bool ok = std::regex_search(message, match, pattern);
    if (ok && match.size() > 1) {
      std::string str = match.str(1);
      long lineNumber = std::stoll(str);
      return lineNumber + 1;
    }

    return -1;
  }

  static bool pickMessage(const char* message, std::string& result)
  {
    std::regex pattern(R"(^[^:]+:\d+:([^:]*:)? (.+))");
    std::cmatch match;

    if (std::regex_search(message, match, pattern) && match.size() > 2) {
      result = match.str(2);
      return true;
    }

    return false;
  }

  static int countLines(const std::string& str)
  {
    return std::count(str.begin(), str.end(), '\n') + 1;
  }
};