#pragma once

#include "ruby.h"

namespace Eng {

class EngineBase {
 public:
  VALUE rbObj;

  EngineBase(VALUE rbObj = Qnil) :
      rbObj(rbObj),
      dirty(false) { }

  inline bool hasRbObj() const
  {
    return rbObj != Qnil;
  }

  inline bool isDirty() const
  {
    return dirty;
  }

  void markAsDirty()
  {
    dirty = true;
  }

  void markAsClean()
  {
    dirty = false;
  }

 private:
  bool dirty;
};

}  // namespace Eng