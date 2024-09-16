#pragma once

#include "AppDefs.h"
#include "RbUtils.hpp"
#include "engnine/Graphics.h"
#include "integrator/Convert.hpp"
#include "ruby.h"

namespace It {

using app::CStr;

class Graphics {
 public:

  static void integrate()
  {
    VALUE graphicsModule = rb_define_module("Graphics");

    rb_define_module_function(graphicsModule, "frame_rate", RUBY_METHOD_FUNC(attrGet_frame_rate), 0);
    rb_define_module_function(graphicsModule, "frame_rate=", RUBY_METHOD_FUNC(attrSet_frame_rate), 1);

    rb_define_module_function(graphicsModule, "frame_count", RUBY_METHOD_FUNC(attrGet_frame_count), 0);
    rb_define_module_function(graphicsModule, "frame_count=", RUBY_METHOD_FUNC(attrSet_frame_count), 1);

    rb_define_module_function(graphicsModule, "update", RUBY_METHOD_FUNC(method_update), 0);
    rb_define_module_function(graphicsModule, "freeze", RUBY_METHOD_FUNC(method_freeze), 0);
    rb_define_module_function(graphicsModule, "transition", RUBY_METHOD_FUNC(method_transition), -1);
    rb_define_module_function(graphicsModule, "frame_reset", RUBY_METHOD_FUNC(method_frame_reset), 0);

    // Extended methods
    rb_define_module_function(graphicsModule, "width", RUBY_METHOD_FUNC(getter_width), 0);
    rb_define_module_function(graphicsModule, "height", RUBY_METHOD_FUNC(getter_height), 0);
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
    unsigned int rate = Eng::Graphics::GetInstance().getFrameRate();
    return INT2FIX(rate);
  }

  static VALUE attrSet_frame_rate(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    unsigned int rate = FIX2INT(value);
    Eng::Graphics::GetInstance().setFrameRate(rate);
    return value;
  }

  /*
    Attr frame_count
  */

  static VALUE attrGet_frame_count(VALUE self)
  {
    unsigned long count = Eng::Graphics::GetInstance().frame_count;
    return INT2NUM(count);
  }

  static VALUE attrSet_frame_count(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    unsigned long count = NUM2INT(value);
    Eng::Graphics::GetInstance().frame_count = count;
    return value;
  }

  /*
    Method update
  */
  static VALUE method_update(VALUE self)
  {
    Eng::Graphics::GetInstance().update();
    return Qnil;
  }

  /*
    Method freeze
  */
  static VALUE method_freeze(VALUE self)
  {
    Eng::Graphics::GetInstance().freeze();
    return Qnil;
  }

  /*
    Method frame_reset
  */
  static VALUE method_transition(int argc, VALUE *argv, VALUE self)
  {
    int duration;
    CStr fileName;
    int vague;

    if (argc == 0) {
      Eng::Graphics::GetInstance().transition();
      return Qnil;
    }

    if (argc == 1) {
      duration = Convert::toCInt2(argv[0]);
      Eng::Graphics::GetInstance().transition(duration);
      return Qnil;
    }
    if (argc == 2) {
      duration = Convert::toCInt2(argv[0]);
      fileName = Convert::toCStr(argv[1]);
      Eng::Graphics::GetInstance().transition(duration, fileName);
      return Qnil;
    }
    if (argc == 3) {
      duration = Convert::toCInt2(argv[0]);
      fileName = Convert::toCStr(argv[1]);
      vague = Convert::toCInt2(argv[2]);
      Eng::Graphics::GetInstance().transition(duration, fileName, vague);
      return Qnil;
    }

    RbUtils::raiseRuntimeException("Bad number of arguments was receive.");
    return Qnil;
  }

  /*
    Method frame_reset
  */
  static VALUE method_frame_reset(VALUE self)
  {
    Eng::Graphics::GetInstance().frame_reset();
    return Qnil;
  }

  /*
    ⇩⇩⇩ Extended Methods ⇩⇩⇩
  */

  static VALUE getter_width(VALUE self)
  {
    return Convert::toRubyNumber(
      Eng::Graphics::GetInstance().getWidth()
    );
  }

  static VALUE getter_height(VALUE self)
  {
    return Convert::toRubyNumber(
      Eng::Graphics::GetInstance().getHeight()
    );
  }
};

}  // namespace It