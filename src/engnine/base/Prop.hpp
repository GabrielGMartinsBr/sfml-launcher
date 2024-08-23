#pragma once

#include "engnine/base/PropNumber.hpp"
namespace Eng {

struct Prop {
  static PropNumber<double> Float(CStr name, double val, EngineBase *inst)
  {
    auto toC = &Convert::toCDouble;
    auto toR = &Convert::toRubyDouble;
    return PropNumber<double>(name, toC, toR, *inst, val);
  }

  static PropNumber<int> FixNum(CStr name, int val, EngineBase *inst)
  {
    auto toC = &Convert::toCInt2;
    auto toR = &Convert::toRubyNumber;
    return PropNumber<int>(name, toC, toR, *inst, val);
  }
};

}  // namespace Eng