#pragma once

#include "AppDefs.h"
#include "ruby.h"

namespace Eng {

using app::CStr;

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

  void setInstanceVar(CStr name, VALUE value)
  {
    if (hasRbObj()) {
      rb_iv_set(rbObj, name, value);
    }
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