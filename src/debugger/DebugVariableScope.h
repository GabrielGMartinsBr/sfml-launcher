#pragma once

#include "AppDefs.h"
#include "StringEnum.hpp"

namespace dbg {

using app::CStr;
using app::StringEnum;

struct DebugVariableScope {
  static const StringEnum GLOBAL;
  static const StringEnum CLASS;
  static const StringEnum INSTANCE;
  static const StringEnum LOCAL;
};

}  // namespace dbg
