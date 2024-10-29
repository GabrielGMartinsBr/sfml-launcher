#pragma once

#include <string>

#include "AppDefs.h"
#include "integrator/Convert.hpp"
#include "ruby.h"

namespace Eng {

using app::CStr;

class EngineBase {
 public:
  VALUE rbObj;

  EngineBase(VALUE rbObj = Qnil) :
      rbObj(rbObj),
      dirty(false)
  {
    bindRubyId();
  }

  void setRbId(VALUE value)
  {
    rbObj = value;
    bindRubyId();
  }

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

  void bindRubyId()
  {
    if (hasRbObj()) {
      setInstanceVar("@rbId", Convert::toRubyString(std::to_string(rbObj)));
    }
  }
};

}  // namespace Eng