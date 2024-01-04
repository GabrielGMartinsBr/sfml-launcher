#pragma once

#include <stdexcept>

#include "base/AppDefs.h"
#include "ruby.h"

struct RbUtils {
  static VALUE createRbString(app::String& str)
  {
    VALUE rbStr = rb_str_new(str.c_str(), str.size());
    if (NIL_P(rbStr)) {
      throw std::runtime_error("Failed to create ruby string.");
    }
    return rbStr;
  }

  static VALUE marshalLoad(VALUE str)
  {
    VALUE arr = rb_marshal_load(str);
    VALUE errorMsg = rb_gv_get("$!");
    if (errorMsg != Qnil) {
      rb_p(errorMsg);
      throw std::runtime_error("Marshal load failed.");
    }
    return arr;
  }

  static long getArraySize(VALUE arr)
  {
    return RARRAY_LEN(arr);
  }
};