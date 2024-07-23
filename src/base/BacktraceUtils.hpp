#pragma once

#include <cstring>
#include <regex>

class BacktraceUtils {

 public:
  static void pickLineNumber(const char* message, long& result)
  {
    if (strcmp(message, "(eval)") == 0) {
      result = 0;
      return;
    }

    std::regex pattern(R"(^[^:]+:(\d+))");
    std::cmatch match;

    bool ok = std::regex_search(message, match, pattern);
    if (ok && match.size() > 1) {
      std::string str = match.str(1);
      long lineNumber = std::stoll(str);
      result = lineNumber + 1;
    } else {
      result = -1;
    }
  }

  static bool pickMessage(std::string& message, std::string& result)
  {
    std::regex pattern(R"(^[^:]+:\d+:([^:]*:)? (.+))");
    std::cmatch match;

    if (std::regex_search(message.c_str(), match, pattern) && match.size() > 2) {
      result = match.str(2);
      return true;
    }

    result = message;

    return false;
  }

  static bool pickMethod(const char* message, std::string& result)
  {
    std::regex pattern(R"(^[^:]+:\d+:([^:]*$))");
    std::cmatch match;

    if (std::regex_search(message, match, pattern) && match.size() > 1) {
      result = ":" + match.str(1);
      return true;
    }

    result = "";

    return false;
  }

  static int countLines(const std::string& str)
  {
    return std::count(str.begin(), str.end(), '\n') + 1;
  }
};