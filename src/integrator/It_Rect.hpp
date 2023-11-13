#pragma once

#include "engnine/Rect.hpp"
#include "ruby.h"

namespace It {

class Rect {
 public:
  static void integrate()
  {
    VALUE rectClass = rb_define_class("Rect", rb_cObject);

    rb_define_method(rectClass, "initialize", RUBY_METHOD_FUNC(method_initialize), 4);

    rb_define_method(rectClass, "x", RUBY_METHOD_FUNC(attrGet_x), 0);
    rb_define_method(rectClass, "x=", RUBY_METHOD_FUNC(attrSet_x), 1);

    rb_define_method(rectClass, "y", RUBY_METHOD_FUNC(attrGet_y), 0);
    rb_define_method(rectClass, "y=", RUBY_METHOD_FUNC(attrSet_y), 1);

    rb_define_method(rectClass, "width", RUBY_METHOD_FUNC(attrGet_width), 0);
    rb_define_method(rectClass, "width=", RUBY_METHOD_FUNC(attrSet_width), 1);

    rb_define_method(rectClass, "height", RUBY_METHOD_FUNC(attrGet_height), 0);
    rb_define_method(rectClass, "height=", RUBY_METHOD_FUNC(attrSet_height), 1);
  }

  static VALUE getRbClass()
  {
    return rb_const_get(rb_cObject, rb_intern("Rect"));
  }

 private:

  static VALUE method_initialize(VALUE self, VALUE _x, VALUE _y, VALUE _w, VALUE _h)
  {
    Check_Type(_x, T_FIXNUM);
    Check_Type(_y, T_FIXNUM);
    Check_Type(_w, T_FIXNUM);
    Check_Type(_h, T_FIXNUM);

    unsigned int x = FIX2INT(_x);
    unsigned int y = FIX2INT(_y);
    unsigned int width = FIX2INT(_w);
    unsigned int height = FIX2INT(_h);

    Eng::Rect *instance = new Eng::Rect(x, y, width, height);
    DATA_PTR(self) = instance;

    return self;
  }

  /*
    Attr x
  */

  static VALUE attrGet_x(VALUE self)
  {
    Eng::Rect *inst = (Eng::Rect *)DATA_PTR(self);
    return INT2FIX(inst->x);
  }

  static VALUE attrSet_x(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    unsigned int x = FIX2INT(value);
    Eng::Rect *inst = (Eng::Rect *)DATA_PTR(self);
    inst->x = x;
    return value;
  }

  /*
    Attr y
  */

  static VALUE attrGet_y(VALUE self)
  {
    Eng::Rect *inst = (Eng::Rect *)DATA_PTR(self);
    return INT2FIX(inst->y);
  }

  static VALUE attrSet_y(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    unsigned int y = FIX2INT(value);
    Eng::Rect *inst = (Eng::Rect *)DATA_PTR(self);
    inst->y = y;
    return value;
  }

  /*
    Attr width
  */

  static VALUE attrGet_width(VALUE self)
  {
    Eng::Rect *inst = (Eng::Rect *)DATA_PTR(self);
    return INT2FIX(inst->width);
  }

  static VALUE attrSet_width(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    unsigned int width = FIX2INT(value);
    Eng::Rect *inst = (Eng::Rect *)DATA_PTR(self);
    inst->width = width;
    return value;
  }

  /*
    Attr height
  */

  static VALUE attrGet_height(VALUE self)
  {
    Eng::Rect *inst = (Eng::Rect *)DATA_PTR(self);
    return INT2FIX(inst->height);
  }

  static VALUE attrSet_height(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    unsigned int height = FIX2INT(value);
    Eng::Rect *inst = (Eng::Rect *)DATA_PTR(self);
    inst->height = height;
    return value;
  }
};

}  // namespace It