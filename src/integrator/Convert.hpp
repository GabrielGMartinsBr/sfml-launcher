#pragma once

#include <memory>
#include <vector>

#include "base/AppDefs.h"
#include "ruby.h"

struct Convert {
  static app::CStr toCStr(VALUE rbStr)
  {
    Check_Type(rbStr, T_STRING);
    return StringValueCStr(rbStr);
  }

  static app::String toCString(VALUE rbStr)
  {
    Check_Type(rbStr, T_STRING);
    return StringValueCStr(rbStr);
  }

  static VALUE toRubyString(const app::String& str)
  {
    VALUE rbStr = rb_str_new(str.c_str(), str.size());
    if (NIL_P(rbStr)) {
      throw std::runtime_error("Failed to create ruby string.");
    }
    return rbStr;
  }

  static int toCInt(VALUE v)
  {
    Check_Type(v, T_FIXNUM);
    return FIX2INT(v);
  }

  static int toCInt2(VALUE v)
  {
    if (TYPE(v) == T_FLOAT) {
      return (int)NUM2DBL(v);
    } else if (TYPE(v) == T_FIXNUM) {
      return FIX2INT(v);
    } else {
      rb_raise(rb_eTypeError, "Expected Float or Fixnum");
      return 0;
    }
  }

  static unsigned int toCUnsignedInt(VALUE v)
  {
    Check_Type(v, T_FIXNUM);
    return FIX2UINT(v);
  }

  static long toCLong(VALUE v)
  {
    Check_Type(v, T_BIGNUM);
    return NUM2LONG(v);
  }

  static double toCDouble(VALUE v)
  {
    Check_Type(v, T_FLOAT);
    return NUM2DBL(v);
  }

  static double toCDouble2(VALUE v)
  {
    if (TYPE(v) == T_FLOAT) {
      return NUM2DBL(v);
    } else if (TYPE(v) == T_FIXNUM) {
      return static_cast<double>(FIX2LONG(v));
    } else {
      rb_raise(rb_eTypeError, "Expected Float or Fixnum");
      return 0.0;
    }
  }

  static VALUE toRubyNumber(int v)
  {
    return INT2FIX(v);
  }

  static VALUE toRubyBigNumber(int v)
  {
    return INT2NUM(v);
  }

  static VALUE toRubyBigNumber(long v)
  {
    return LONG2NUM(v);
  }

  static VALUE uToRubyNumber(unsigned int v)
  {
    return UINT2NUM(v);
  }

  static VALUE uToRubyBigNumber(unsigned long v)
  {
    return ULONG2NUM(v);
  }

  static VALUE toRubyDouble(double v)
  {
    return rb_float_new(v);
  }

  inline static const bool toCBool(VALUE v)
  {
    return RTEST(v);
  }

  inline static const VALUE toRubyBool(const bool v)
  {
    return v ? Qtrue : Qfalse;
  }

  static app::StrVectorPtr toCStringVector(VALUE val)
  {
    app::StrVectorPtr vec = nullptr;

    if (TYPE(val) == T_STRING) {
      vec = std::make_unique<app::Vector<app::String>>(1);
      vec->at(0) = Convert::toCStr(val);
      return vec;
    }

    if (TYPE(val) != T_ARRAY) {
      rb_raise(rb_eTypeError, "Expected String or Array");
      return vec;
    }

    int length = RARRAY_LEN(val);
    vec = std::make_unique<app::Vector<app::String>>(length);
    VALUE entry;

    for (int i = 0; i < length; i++) {
      entry = rb_ary_entry(val, i);
      vec->at(i) = Convert::toCStr(entry);
    }

    return std::move(vec);
  }

  static app::StrVectorPtr toCStringVector2(VALUE arr)
  {
    app::StrVectorPtr vec = nullptr;

    if (TYPE(arr) != T_ARRAY) {
      rb_raise(rb_eTypeError, "Expected String or Array");
      return vec;
    }

    int length = RARRAY_LEN(arr);
    vec = std::make_unique<app::StrVector>(length);
    VALUE entry;

    for (int i = 0; i < length; i++) {
      entry = rb_ary_entry(arr, i);
      vec->at(i) = Convert::toCStr(entry);
    }

    return std::move(vec);
  }

  static VALUE toRubyStringArray(const app::Vector<app::CStr>& vec)
  {
    int length = vec.size();
    VALUE rubyArray = rb_ary_new();
    VALUE rbStr;

    for (int i = 0; i < length; i++) {
      rbStr = Convert::toRubyString(vec.at(i));
      rb_ary_push(rubyArray, rbStr);
    }

    return rubyArray;
  }

  static VALUE toRubyStringArray(const app::StrVector& vec)
  {
    int length = vec.size();
    VALUE rubyArray = rb_ary_new();
    VALUE rbStr;

    for (int i = 0; i < length; i++) {
      rbStr = Convert::toRubyString(vec.at(i));
      rb_ary_push(rubyArray, rbStr);
    }

    return rubyArray;
  }
};