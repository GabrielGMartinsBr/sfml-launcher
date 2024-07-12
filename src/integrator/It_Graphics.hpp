#pragma once

#include "engnine/Graphics.hpp"
#include "ruby.h"

namespace It {

class Graphics {
 public:

  static void integrate()
  {
    VALUE graphicsClass = rb_define_class("Graphics", rb_cObject);

    rb_define_module_function(graphicsClass, "frame_rate", RUBY_METHOD_FUNC(attrGet_frame_rate), 0);
    rb_define_module_function(graphicsClass, "frame_rate=", RUBY_METHOD_FUNC(attrSet_frame_rate), 1);

    rb_define_module_function(graphicsClass, "frame_count", RUBY_METHOD_FUNC(attrGet_frame_count), 0);
    rb_define_module_function(graphicsClass, "frame_count=", RUBY_METHOD_FUNC(attrSet_frame_count), 1);

    rb_define_module_function(graphicsClass, "update", RUBY_METHOD_FUNC(method_update), 0);
    rb_define_module_function(graphicsClass, "transition", RUBY_METHOD_FUNC(method_transition), -1);
    rb_define_module_function(graphicsClass, "frame_reset", RUBY_METHOD_FUNC(method_frame_reset), 0);
  }

  static VALUE getRbClass()
  {
    return rb_const_get(rb_cObject, rb_intern("Graphics"));
  }

 private:

  /*
    Attr frame_rate
  */

  static VALUE attrGet_frame_rate(VALUE self)
  {
    unsigned int rate = Eng::Graphics::getInstance().getFrameRate();
    return INT2FIX(rate);
  }

  static VALUE attrSet_frame_rate(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    unsigned int rate = FIX2INT(value);
    Eng::Graphics::getInstance().setFrameRate(rate);
    return value;
  }

  /*
    Attr frame_count
  */

  static VALUE attrGet_frame_count(VALUE self)
  {
    unsigned long count = Eng::Graphics::getInstance().frame_count;
    return INT2NUM(count);
  }

  static VALUE attrSet_frame_count(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    unsigned long count = NUM2INT(value);
    Eng::Graphics::getInstance().frame_count = count;
    return value;
  }

  /*
    Method update
  */
  static VALUE method_update(VALUE self)
  {
    Eng::Graphics::getInstance().update();
    return Qnil;
  }

  /*
    Method frame_reset
  */
  static VALUE method_transition(VALUE self)
  {
    Eng::Graphics::getInstance().transition();
    return Qnil;
  }

  /*
    Method frame_reset
  */
  static VALUE method_frame_reset(VALUE self)
  {
    Eng::Graphics::getInstance().frame_reset();
    return Qnil;
  }
};

}  // namespace It