#pragma once

#include "base/AppDefs.h"
#include "ruby.h"

struct Convert {
  static app::CStr toCString(VALUE rbStr)
  {
    Check_Type(rbStr, T_STRING);
    return StringValueCStr(rbStr);
  }

  static VALUE toCInt(VALUE v)
  {
    Check_Type(v, T_FIXNUM);
    return FIX2INT(v);
  }

  static VALUE toRubyNumber(int v)
  {
    return INT2FIX(v);
  }
};