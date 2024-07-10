#pragma once

#include "Convert.hpp"
#include "engnine/Color.hpp"
#include "ruby.h"

namespace It {

class Color {
 public:

  static void integrate()
  {
    VALUE colorClass = rb_define_class("Color", rb_cObject);

    rb_define_module_function(colorClass, "_load", RUBY_METHOD_FUNC(method_load), 1);

    rb_define_method(colorClass, "initialize", RUBY_METHOD_FUNC(method_initialize), -1);

    // rb_define_attr(rbc_Color, "red", 1, 1);

    rb_define_method(colorClass, "red", RUBY_METHOD_FUNC(attrGet_red), 0);
    rb_define_method(colorClass, "red=", RUBY_METHOD_FUNC(attrSet_red), 1);

    rb_define_method(colorClass, "green", RUBY_METHOD_FUNC(attrGet_green), 0);
    rb_define_method(colorClass, "green=", RUBY_METHOD_FUNC(attrSet_green), 1);

    rb_define_method(colorClass, "blue", RUBY_METHOD_FUNC(attrGet_blue), 0);
    rb_define_method(colorClass, "blue=", RUBY_METHOD_FUNC(attrSet_blue), 1);

    rb_define_method(colorClass, "set", RUBY_METHOD_FUNC(method_set), 3);
  }

  static VALUE getRbClass()
  {
    return rb_const_get(rb_cObject, rb_intern("Color"));
  }

 private:

  static VALUE method_load(VALUE self, VALUE marshaled_data)
  {
    Check_Type(marshaled_data, T_STRING);
    const char *data = RSTRING_PTR(marshaled_data);
    int len = RSTRING_LEN(marshaled_data);

    Eng::Color *color = Eng::Color::deserialize(data, len);

    VALUE table_obj = Data_Wrap_Struct(self, NULL, free, color);
    return table_obj;
  }

  static VALUE alloc(VALUE self)
  {
    return self;
  }

  static VALUE method_initialize(int argc, VALUE *argv, VALUE self)
  {
    Eng::Color *instance;
    VALUE rb_r, rb_g, rb_b, rb_a;

    if (argc == 3) {
      rb_scan_args(argc, argv, "3", &rb_r, &rb_g, &rb_b);
      int r = Convert::toCInt(rb_r);
      int g = Convert::toCInt(rb_g);
      int b = Convert::toCInt(rb_b);

      instance = new Eng::Color(r, g, b);
      instance->ptr = self;

      DATA_PTR(self) = instance;
      return self;
    }

    if (argc == 4) {
      rb_scan_args(argc, argv, "4", &rb_r, &rb_g, &rb_b, &rb_a);
      int r = Convert::toCInt(rb_r);
      int g = Convert::toCInt(rb_g);
      int b = Convert::toCInt(rb_b);
      int a = Convert::toCInt(rb_a);

      instance = new Eng::Color(r, g, b, a);
      instance->ptr = self;

      DATA_PTR(self) = instance;
      return self;
    }

    return Qnil;
  }

  static VALUE method_set(VALUE self, VALUE p_r, VALUE p_g, VALUE p_b)
  {
    Eng::Color *inst = (Eng::Color *)DATA_PTR(self);

    Check_Type(p_r, T_FIXNUM);
    Check_Type(p_g, T_FIXNUM);
    Check_Type(p_b, T_FIXNUM);

    unsigned int r = FIX2INT(p_r);
    unsigned int g = FIX2INT(p_g);
    unsigned int b = FIX2INT(p_b);

    inst->set(r, g, b);

    return Qnil;
  }

  static VALUE attrGet_red(VALUE self)
  {
    Eng::Color *inst = (Eng::Color *)DATA_PTR(self);
    return INT2FIX(inst->red);
  }

  static VALUE attrSet_red(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    unsigned int r = FIX2INT(value);
    Eng::Color *inst = (Eng::Color *)DATA_PTR(self);
    inst->red = r;
    return value;
  }

  static VALUE attrGet_green(VALUE self)
  {
    Eng::Color *inst = (Eng::Color *)DATA_PTR(self);
    return INT2FIX(inst->green);
  }

  static VALUE attrSet_green(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    unsigned int r = FIX2INT(value);
    Eng::Color *inst = (Eng::Color *)DATA_PTR(self);
    inst->green = r;
    return value;
  }

  static VALUE attrGet_blue(VALUE self)
  {
    Eng::Color *inst = (Eng::Color *)DATA_PTR(self);
    return INT2FIX(inst->blue);
  }

  static VALUE attrSet_blue(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    unsigned int r = FIX2INT(value);
    Eng::Color *inst = (Eng::Color *)DATA_PTR(self);
    inst->blue = r;
    return value;
  }
};

}  // namespace It