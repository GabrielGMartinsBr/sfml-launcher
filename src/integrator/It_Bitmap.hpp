#pragma once

#include "engnine/Bitmap.hpp"
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
  }

 private:

  static VALUE method_initialize(VALUE self, VALUE _width, VALUE _height)
  {
    Check_Type(_width, T_FIXNUM);
    Check_Type(_height, T_FIXNUM);

    int width = FIX2INT(_width);
    int height = FIX2INT(_height);

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
};

}  // namespace It