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
};

}  // namespace It