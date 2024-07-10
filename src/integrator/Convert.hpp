#pragma once

#include "base/AppDefs.h"
#include "ruby.h"

struct Convert {
  static app::CStr toCStr(VALUE rbStr)
  {
    Check_Type(rbStr, T_STRING);
    return StringValueCStr(rbStr);
  }

  static VALUE toRubyString(app::String str)
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

  static VALUE toRubyNumber(int v)
  {
    return INT2FIX(v);
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