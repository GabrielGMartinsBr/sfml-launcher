#pragma once

#include <iostream>

#include "engnine/Color.hpp"
#include "ruby.h"

namespace It {

class Color {
 public:

  static void integrate()
  {
    VALUE colorClass = rb_define_class("Color", rb_cObject);

    rb_define_method(colorClass, "initialize", RUBY_METHOD_FUNC(method_initialize), 3);

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

  static VALUE alloc(VALUE self)
  {
    return self;
  }

  static VALUE method_initialize(VALUE self, VALUE p_r, VALUE p_g, VALUE p_b)
  {
    Check_Type(p_r, T_FIXNUM);
    Check_Type(p_g, T_FIXNUM);
    Check_Type(p_b, T_FIXNUM);

    unsigned int r = FIX2INT(p_r);
    unsigned int g = FIX2INT(p_g);
    unsigned int b = FIX2INT(p_b);

    Eng::Color *instance = new Eng::Color(r, g, b);
    DATA_PTR(self) = instance;

    return self;
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