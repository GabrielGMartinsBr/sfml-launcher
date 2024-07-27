#pragma once

#include "ruby.h"

namespace Eng {

class EngineBase {
 public:
  VALUE rbObj;

  EngineBase(VALUE rbObj = Qnil) :
      rbObj(rbObj) { }
};

}  // namespace Eng