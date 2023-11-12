#pragma once

#include "engnine/Bitmap.hpp"
#include "integrator/It_Color.hpp"
#include "ruby.h"

namespace It {

class Bitmap {

 public:

  static void integrate()
  {
    VALUE bitmapClass = rb_define_class("Bitmap", rb_cObject);

    rb_define_method(bitmapClass, "initialize", RUBY_METHOD_FUNC(method_initialize), 2);

    rb_define_method(bitmapClass, "width", RUBY_METHOD_FUNC(method_width), 0);
    rb_define_method(bitmapClass, "height", RUBY_METHOD_FUNC(method_height), 0);

    rb_define_method(bitmapClass, "get_pixel", RUBY_METHOD_FUNC(method_get_pixel), 2);
    rb_define_method(bitmapClass, "set_pixel", RUBY_METHOD_FUNC(method_set_pixel), 3);

    rb_define_method(bitmapClass, "dispose", RUBY_METHOD_FUNC(method_dispose), 0);
    rb_define_method(bitmapClass, "disposed", RUBY_METHOD_FUNC(method_disposed), 0);
  }

  static VALUE getRbClass()
  {
    return rb_const_get(rb_cObject, rb_intern("Bitmap"));
  }

 private:

  static VALUE method_initialize(VALUE self, VALUE _width, VALUE _height)
  {
    Check_Type(_width, T_FIXNUM);
    Check_Type(_height, T_FIXNUM);

    unsigned int width = FIX2INT(_width);
    unsigned int height = FIX2INT(_height);

    Eng::Bitmap *inst = new Eng::Bitmap(width, height);
    DATA_PTR(self) = inst;

    return self;
  }

  static VALUE method_width(VALUE self)
  {
    Eng::Bitmap *inst = (Eng::Bitmap *)DATA_PTR(self);
    return INT2NUM(inst->width);
  }

  static VALUE method_height(VALUE self)
  {
    Eng::Bitmap *inst = (Eng::Bitmap *)DATA_PTR(self);
    return INT2NUM(inst->height);
  }

  static VALUE method_get_pixel(VALUE self, VALUE _x, VALUE _y)
  {
    Check_Type(_x, T_FIXNUM);
    Check_Type(_y, T_FIXNUM);

    unsigned int x = NUM2UINT(_x);
    unsigned int y = NUM2UINT(_y);

    Eng::Bitmap *inst = (Eng::Bitmap *)DATA_PTR(self);
    Eng::Color *color = inst->get_pixel(x, y);

    VALUE colorClass = Color::getRbClass();
    return Data_Wrap_Struct(colorClass, 0, free, color);
  }

  static VALUE method_set_pixel(VALUE self, VALUE _x, VALUE _y, VALUE _color)
  {
    Check_Type(_x, T_FIXNUM);
    Check_Type(_y, T_FIXNUM);
    Check_Type(_color, T_OBJECT);

    unsigned int x = NUM2UINT(_x);
    unsigned int y = NUM2UINT(_y);

    Eng::Bitmap *inst = (Eng::Bitmap *)DATA_PTR(self);
    Eng::Color *color = (Eng::Color *)DATA_PTR(_color);

    inst->set_pixel(x, y, color);

    return Qnil;
  }

  static VALUE method_dispose(VALUE self)
  {
    Eng::Bitmap *inst = (Eng::Bitmap *)DATA_PTR(self);
    inst->dispose();
    return Qnil;
  }

  static VALUE method_disposed(VALUE self)
  {
    Eng::Bitmap *inst = (Eng::Bitmap *)DATA_PTR(self);
    bool isDisposed = inst->disposed();
    return isDisposed ? Qtrue : Qfalse;
  }
};

}  // namespace It