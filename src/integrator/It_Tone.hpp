#pragma once

#include "RbUtils.hpp"
#include "engnine/Tone.hpp"
#include "integrator/Convert.hpp"
#include "ruby.h"

namespace It {

class Tone {
 public:

  static void integrate()
  {
    VALUE rbc_Tone = rb_define_class("Tone", rb_cObject);

    rb_define_module_function(rbc_Tone, "_load", RUBY_METHOD_FUNC(method_load), 1);

    rb_define_method(rbc_Tone, "initialize", RUBY_METHOD_FUNC(method_initialize), -1);

    rb_define_method(rbc_Tone, "red", RUBY_METHOD_FUNC(attrGet_red), 0);
    rb_define_method(rbc_Tone, "red=", RUBY_METHOD_FUNC(attrSet_red), 1);

    rb_define_method(rbc_Tone, "green", RUBY_METHOD_FUNC(attrGet_green), 0);
    rb_define_method(rbc_Tone, "green=", RUBY_METHOD_FUNC(attrSet_green), 1);

    rb_define_method(rbc_Tone, "blue", RUBY_METHOD_FUNC(attrGet_blue), 0);
    rb_define_method(rbc_Tone, "blue=", RUBY_METHOD_FUNC(attrSet_blue), 1);

    rb_define_method(rbc_Tone, "set", RUBY_METHOD_FUNC(method_set), 3);
  }

  // Utils

  static VALUE getRbClass()
  {
    return rb_const_get(rb_cObject, rb_intern("Tone"));
  }

  static VALUE createRubyObject(Eng::Tone *inst)
  {
    return Data_Wrap_Struct(getRbClass(), 0, free, inst);
  }

  static VALUE getRubyObject(Eng::Tone *inst)
  {
    if (inst == nullptr) {
      return Qnil;
    }
    if (inst->ptr == Qnil) {
      inst->ptr = createRubyObject(inst);
    }
    return inst->ptr;
  }

  static Eng::Tone *getObjectValue(VALUE rbObj)
  {
    return (Eng::Tone *)DATA_PTR(rbObj);
  }

 private:
  static VALUE method_load(VALUE self, VALUE marshaled_data)
  {
    Check_Type(marshaled_data, T_STRING);
    const char *data = RSTRING_PTR(marshaled_data);
    int len = RSTRING_LEN(marshaled_data);

    Eng::Tone *tone = Eng::Tone::deserialize(data, len);

    VALUE rubyObj = Data_Wrap_Struct(self, NULL, free, tone);
    return rubyObj;
  }

  static VALUE method_initialize(int argc, VALUE *argv, VALUE self)
  {
    VALUE _red, _green, _blue, _gray;
    Eng::Tone *instance;

    if (argc == 3) {
      rb_scan_args(argc, argv, "3", &_red, &_green, &_blue);
      int red = Convert::toCInt(_red);
      int green = Convert::toCInt(_green);
      int blue = Convert::toCInt(_blue);
      instance = new Eng::Tone(red, green, blue);
    } else if (argc == 4) {
      rb_scan_args(argc, argv, "4", &_red, &_green, &_blue, &_gray);
      int red = Convert::toCInt(_red);
      int green = Convert::toCInt(_green);
      int blue = Convert::toCInt(_blue);
      int gray = Convert::toCInt(_gray);
      instance = new Eng::Tone(red, green, blue, gray);
    } else {
      RbUtils::raiseRuntimeException(
        "Tone initialize takes 3 or 4 argument, but " + std::to_string(argc) + " were received."
      );
      return Qnil;
    }

    DATA_PTR(self) = instance;
    return self;
  }

  static VALUE method_set(VALUE self, VALUE p_r, VALUE p_g, VALUE p_b)
  {
    Eng::Tone *inst = (Eng::Tone *)DATA_PTR(self);

    Check_Type(p_r, T_FIXNUM);
    Check_Type(p_g, T_FIXNUM);
    Check_Type(p_b, T_FIXNUM);

    int r = FIX2INT(p_r);
    int g = FIX2INT(p_g);
    int b = FIX2INT(p_b);

    inst->set(r, g, b);

    return Qnil;
  }

  static VALUE attrGet_red(VALUE self)
  {
    Eng::Tone *inst = (Eng::Tone *)DATA_PTR(self);
    return INT2NUM(inst->red);
  }

  static VALUE attrSet_red(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    int r = NUM2INT(value);
    Eng::Tone *inst = (Eng::Tone *)DATA_PTR(self);
    inst->red = r;
    return value;
  }

  static VALUE attrGet_green(VALUE self)
  {
    Eng::Tone *inst = (Eng::Tone *)DATA_PTR(self);
    return INT2NUM(inst->green);
  }

  static VALUE attrSet_green(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    int r = NUM2INT(value);
    Eng::Tone *inst = (Eng::Tone *)DATA_PTR(self);
    inst->green = r;
    return value;
  }

  static VALUE attrGet_blue(VALUE self)
  {
    Eng::Tone *inst = (Eng::Tone *)DATA_PTR(self);
    return INT2NUM(inst->blue);
  }

  static VALUE attrSet_blue(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    int r = NUM2INT(value);
    Eng::Tone *inst = (Eng::Tone *)DATA_PTR(self);
    inst->blue = r;
    return value;
  }
};

}  // namespace It