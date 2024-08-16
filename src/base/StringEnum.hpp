#pragma once

#include <cstring>

#include "AppDefs.h"

namespace app {
struct StringEnum {
  StringEnum(CStr value) :
      value(value) { }

  bool operator==(const StringEnum &other) const
  {
    return std::strcmp(value, other.value) == 0;
  }

  bool operator==(const CStr &other) const
  {
    return std::strcmp(value, other) == 0;
  }

  bool operator==(const String &other) const
  {
    return std::strcmp(value, other.c_str()) == 0;
  }

  inline const CStr &toString() const
  {
    return value;
  }

 private:
  const CStr value;
};
}  // namespace app
