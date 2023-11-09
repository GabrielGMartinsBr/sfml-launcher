#pragma once

#include <iostream>

#include "engnine/Color.hpp"
#include "ruby.h"

namespace It {

class Color {
 public:

  static void integrate()
  {
    VALUE rbc_Color = rb_define_class("Color", rb_cObject);

    rb_define_method(rbc_Color, "initialize", RUBY_METHOD_FUNC(method_initialize), 3);

    // rb_define_attr(rbc_Color, "red", 1, 1);

    rb_define_method(rbc_Color, "red", RUBY_METHOD_FUNC(attrGet_red), 0);
    rb_define_method(rbc_Color, "red=", RUBY_METHOD_FUNC(attrSet_red), 1);

    rb_define_method(rbc_Color, "green", RUBY_METHOD_FUNC(attrGet_green), 0);
    rb_define_method(rbc_Color, "green=", RUBY_METHOD_FUNC(attrSet_green), 1);

    rb_define_method(rbc_Color, "blue", RUBY_METHOD_FUNC(attrGet_blue), 0);
    rb_define_method(rbc_Color, "blue=", RUBY_METHOD_FUNC(attrSet_blue), 1);

    rb_define_method(rbc_Color, "set", RUBY_METHOD_FUNC(method_set), 3);
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

    int r = FIX2INT(p_r);
    int g = FIX2INT(p_g);
    int b = FIX2INT(p_b);

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

    int r = FIX2INT(p_r);
    int g = FIX2INT(p_g);
    int b = FIX2INT(p_b);

    inst->set(r, g, b);

    return Qnil;
  }

  static VALUE attrGet_red(VALUE self)
  {
    Eng::Color *inst = (Eng::Color *)DATA_PTR(self);
    return INT2NUM(inst->red);
  }

  static VALUE attrSet_red(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    int r = NUM2INT(value);
    Eng::Color *inst = (Eng::Color *)DATA_PTR(self);
    inst->red = r;
    return value;
  }

  static VALUE attrGet_green(VALUE self)
  {
    Eng::Color *inst = (Eng::Color *)DATA_PTR(self);
    return INT2NUM(inst->green);
  }

  static VALUE attrSet_green(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    int r = NUM2INT(value);
    Eng::Color *inst = (Eng::Color *)DATA_PTR(self);
    inst->green = r;
    return value;
  }

  static VALUE attrGet_blue(VALUE self)
  {
    Eng::Color *inst = (Eng::Color *)DATA_PTR(self);
    return INT2NUM(inst->blue);
  }

  static VALUE attrSet_blue(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    int r = NUM2INT(value);
    Eng::Color *inst = (Eng::Color *)DATA_PTR(self);
    inst->blue = r;
    return value;
  }
};

}  // namespace It