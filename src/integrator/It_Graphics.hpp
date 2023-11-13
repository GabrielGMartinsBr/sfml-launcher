#pragma once

#include "engnine/Graphics.hpp"
#include "ruby.h"

namespace It {

class Graphics {
 public:

  static void integrate()
  {
    VALUE graphicsClass = rb_define_class("Graphics", rb_cObject);

    rb_define_module_function(graphicsClass, "frame_count", RUBY_METHOD_FUNC(attrGet_frame_count), 0);
    rb_define_module_function(graphicsClass, "frame_count=", RUBY_METHOD_FUNC(attrSet_frame_count), 1);

    rb_define_module_function(graphicsClass, "update", RUBY_METHOD_FUNC(method_update), 0);
  }

  static VALUE getRbClass()
  {
    return rb_const_get(rb_cObject, rb_intern("Graphics"));
  }

 private:

  static VALUE attrGet_frame_count(VALUE self)
  {
    unsigned int count = Eng::Graphics::getInstance().frame_count;
    return INT2NUM(count);
  }

  static VALUE attrSet_frame_count(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    unsigned int count = NUM2INT(value);
    Eng::Graphics::getInstance().frame_count = count;
    return value;
  }

  static VALUE method_update(VALUE self)
  {
    Eng::Graphics::getInstance().update();
    return Qnil;
  }
};

}  // namespace It