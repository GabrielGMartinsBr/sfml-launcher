#pragma once

#include "ruby.h"

namespace Eng {

class EngineBase {
 public:
  VALUE rbObj;

  EngineBase(VALUE rbObj = Qnil) :
      rbObj(rbObj) { }

  inline bool hasRbObj() const
  {
    return rbObj != Qnil;
  }
};

}  // namespace Eng