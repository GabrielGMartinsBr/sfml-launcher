#pragma once

#include <ruby.h>

#include "AppDefs.h"

namespace dbg {

using app::String;
using app::StrVector;
using app::StrVectorPtr;
using app::UInt;
using app::WString;

struct UnserializeUtils {
  static void parserSetVariableValueArgs(const String& msg, VALUE& varRId, String& varType, String& varValue)
  {
    String varRIdBuff = "";
    String sizeBuff = "";
    varType = "";

    int i = 0;
    char c;

    while (i < msg.length()) {
      c = msg.substr(i, i + 1)[0];
      if (c == '|') {
        i++;
        break;
      }
      varRIdBuff += c;
      i++;
    }

    while (i < msg.length()) {
      c = msg.substr(i, i + 1)[0];
      if (c == ':') {
        i++;
        break;
      }
      varType += c;
      i++;
    }

    while (i < msg.length()) {
      c = msg.substr(i, i + 1)[0];
      if (c == '|') {
        i++;
        break;
      }
      sizeBuff += c;
      i++;
    }

    varRId = static_cast<VALUE>(std::stoul(varRIdBuff));
    UInt size = static_cast<UInt>(std::stoul(sizeBuff));

    varValue = msg.substr(i, i + size);
  }
};

}  // namespace dbg