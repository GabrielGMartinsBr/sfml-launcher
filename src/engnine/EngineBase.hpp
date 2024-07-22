#pragma once

#include "ruby.h"

namespace Eng {

class EngineBase {
 public:
  VALUE ptr;

  EngineBase() :
      ptr(Qnil) { }
};

}  // namespace Eng