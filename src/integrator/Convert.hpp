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

  static app::Vector<app::String>* toCStringVector(VALUE arr)
  {
    if (TYPE(arr) == T_STRING) {
      std::vector<std::string>* vec = new std::vector<std::string>(1);
      vec->at(0) = arr;
      return vec;
    }

    if (TYPE(arr) != T_ARRAY) {
      rb_raise(rb_eTypeError, "Expected String or Array");
      return nullptr;
    }

    int length = RARRAY_LEN(arr);
    std::vector<std::string>* vec = new std::vector<std::string>(length);
    const char* str;
    VALUE entry;

    for (int i = 0; i < length; i++) {
      entry = rb_ary_entry(arr, i);
      str = Convert::toCStr(entry);
      vec->at(i) = str;
    }

    return vec;
  }

  static app::UPtr<app::Vector<app::String>> toCStringVector2(VALUE arr)
  {
    std::unique_ptr<app::Vector<app::String>> vec = nullptr;

    if (TYPE(arr) != T_ARRAY) {
      rb_raise(rb_eTypeError, "Expected String or Array");
      return vec;
    }

    int length = RARRAY_LEN(arr);
    vec = std::make_unique<app::Vector<app::String>>(length);
    VALUE entry;

    for (int i = 0; i < length; i++) {
      entry = rb_ary_entry(arr, i);
      vec->at(i) = Convert::toCStr(entry);
    }

    return vec;
  }

  static VALUE toRubyStringArray(app::Vector<app::CStr>* vec)
  {
    int length = vec->size();
    VALUE rubyArray = rb_ary_new();
    VALUE rbStr;

    for (int i = 0; i < length; i++) {
      rbStr = Convert::toRubyString(vec->at(i));
      rb_ary_push(rubyArray, rbStr);
    }

    return rubyArray;
  }

  static VALUE toRubyStringArray(app::Vector<app::String>* vec)
  {
    int length = vec->size();
    VALUE rubyArray = rb_ary_new();
    VALUE rbStr;

    for (int i = 0; i < length; i++) {
      rbStr = Convert::toRubyString(vec->at(i));
      rb_ary_push(rubyArray, rbStr);
    }

    return rubyArray;
  }
};