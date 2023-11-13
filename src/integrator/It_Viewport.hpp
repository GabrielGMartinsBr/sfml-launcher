#pragma once

#include <stdexcept>

#include "engnine/Rect.hpp"
#include "engnine/Viewport.hpp"
#include "integrator/It_Rect.hpp"
#include "ruby.h"

namespace It {

class Viewport {

 public:

  static void integrate()
  {
    VALUE viewportClass = rb_define_class("Viewport", rb_cObject);

    rb_define_method(viewportClass, "initialize", RUBY_METHOD_FUNC(method_initialize), -1);

    rb_define_method(viewportClass, "rect", RUBY_METHOD_FUNC(attrGet_rect), 0);
    rb_define_method(viewportClass, "rect=", RUBY_METHOD_FUNC(attrSet_rect), 1);
  }

 private:

  /*
    Method initialize
  */

  static VALUE method_initialize(int argc, VALUE *argv, VALUE self)
  {
    if (argc == 4) {
      return overload_initialize1(argc, argv, self);
    } else if (argc == 1) {
      return overload_initialize2(argc, argv, self);
    } else {
      throw std::runtime_error("Failed to initialize bitmap.");
    }
  }

  static VALUE overload_initialize1(int argc, VALUE *argv, VALUE self)
  {
    VALUE _x, _y, _width, _height;
    rb_scan_args(argc, argv, "4", &_x, &_y, &_width, &_height);

    Check_Type(_x, T_FIXNUM);
    Check_Type(_y, T_FIXNUM);
    Check_Type(_width, T_FIXNUM);
    Check_Type(_height, T_FIXNUM);

    int x = FIX2INT(_x);
    int y = FIX2INT(_y);
    unsigned int width = FIX2INT(_width);
    unsigned int height = FIX2INT(_height);

    Eng::Viewport *inst = new Eng::Viewport(x, y, width, height);
    DATA_PTR(self) = inst;

    return self;
  }

  static VALUE overload_initialize2(int argc, VALUE *argv, VALUE self)
  {
    VALUE _rect;
    rb_scan_args(argc, argv, "1", &_rect);

    Check_Type(_rect, T_OBJECT);

    Eng::Rect *rect = (Eng::Rect *)DATA_PTR(_rect);

    Eng::Viewport *inst = new Eng::Viewport(rect);
    DATA_PTR(self) = inst;

    return self;
  }

  /*
    Attr rect
  */

  static VALUE attrGet_rect(VALUE self)
  {
    Eng::Viewport *inst = (Eng::Viewport *)DATA_PTR(self);
    Eng::Rect *rect = inst->getRect();

    if (rect == nullptr) {
      return Qnil;
    }

    VALUE rectClass = Rect::getRbClass();
    return Data_Wrap_Struct(rectClass, 0, free, rect);
  }

  static VALUE attrSet_rect(VALUE self, VALUE value)
  {
    Eng::Rect *rect = (Eng::Rect *)DATA_PTR(value);
    if (rect == nullptr) {
      throw std::runtime_error("invalid rect pointer");
    }

    Eng::Viewport *inst = (Eng::Viewport *)DATA_PTR(self);
    inst->setRect(rect);

    return Qnil;
  }
};

}  // namespace It