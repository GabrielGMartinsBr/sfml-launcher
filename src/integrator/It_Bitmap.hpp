#pragma once

#include <stdexcept>

#include "engnine/Bitmap.hpp"
#include "integrator/It_Color.hpp"
#include "ruby.h"

namespace It {

class Bitmap {

 public:

  static void integrate()
  {
    VALUE bitmapClass = rb_define_class("Bitmap", rb_cObject);

    rb_define_method(bitmapClass, "initialize", RUBY_METHOD_FUNC(method_initialize), -1);

    rb_define_method(bitmapClass, "dispose", RUBY_METHOD_FUNC(method_dispose), 0);
    rb_define_method(bitmapClass, "disposed", RUBY_METHOD_FUNC(method_disposed), 0);

    rb_define_method(bitmapClass, "width", RUBY_METHOD_FUNC(method_width), 0);
    rb_define_method(bitmapClass, "height", RUBY_METHOD_FUNC(method_height), 0);

    rb_define_method(bitmapClass, "get_pixel", RUBY_METHOD_FUNC(method_get_pixel), 2);
    rb_define_method(bitmapClass, "set_pixel", RUBY_METHOD_FUNC(method_set_pixel), 3);

    rb_define_method(bitmapClass, "fill_rect", RUBY_METHOD_FUNC(method_fill_rect), -1);
  }

  static VALUE getRbClass()
  {
    return rb_const_get(rb_cObject, rb_intern("Bitmap"));
  }

 private:

  /*
    Method initialize
  */

  static VALUE method_initialize(int argc, VALUE *argv, VALUE self)
  {
    if (argc == 2) {
      VALUE _width, _height;
      rb_scan_args(argc, argv, "2", &_width, &_height);
      return initializeWithDimensions(self, _width, _height);
    } else if (argc == 1) {
      VALUE _fileName;
      rb_scan_args(argc, argv, "1", &_fileName);
      return initializeWithImage(self, _fileName);
    } else {
      throw std::runtime_error("Failed to initialize bitmap.");
    }
  }

  /*
    Initializer with dimensions
  */

  static VALUE initializeWithDimensions(VALUE self, VALUE _width, VALUE _height)
  {
    Check_Type(_width, T_FIXNUM);
    Check_Type(_height, T_FIXNUM);

    unsigned int width = FIX2INT(_width);
    unsigned int height = FIX2INT(_height);
    Eng::Bitmap *inst = new Eng::Bitmap(width, height);

    DATA_PTR(self) = inst;
    return self;
  }

  /*
    Initializer with image
  */

  static VALUE initializeWithImage(VALUE self, VALUE _fileName)
  {
    Check_Type(_fileName, T_STRING);

    const char *fileName = StringValuePtr(_fileName);
    Eng::Bitmap *inst = new Eng::Bitmap(fileName);

    DATA_PTR(self) = inst;
    return self;
  }

  /*
    Method dispose
  */

  static VALUE method_dispose(VALUE self)
  {
    Eng::Bitmap *inst = (Eng::Bitmap *)DATA_PTR(self);
    inst->dispose();
    return Qnil;
  }

  /*
    Method disposed
  */

  static VALUE method_disposed(VALUE self)
  {
    Eng::Bitmap *inst = (Eng::Bitmap *)DATA_PTR(self);
    bool isDisposed = inst->disposed();
    return isDisposed ? Qtrue : Qfalse;
  }

  /*
    Method get_width
  */

  static VALUE method_width(VALUE self)
  {
    Eng::Bitmap *inst = (Eng::Bitmap *)DATA_PTR(self);
    return INT2NUM(inst->width);
  }

  /*
    Method get_height
  */

  static VALUE method_height(VALUE self)
  {
    Eng::Bitmap *inst = (Eng::Bitmap *)DATA_PTR(self);
    return INT2NUM(inst->height);
  }

  /*
    Method get_pixel
  */

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

  /*
    Method set_pixel
  */

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

  /*
    Method initialize
  */

  static VALUE method_fill_rect(int argc, VALUE *argv, VALUE self)
  {
    if (argc == 5) {
      return overload_fill_rect1(argc, argv, self);
    } else if (argc == 2) {
      return overload_fill_rect2(argc, argv, self);
    } else {
      throw std::runtime_error("Failed to initialize bitmap.");
    }
  }

  static VALUE overload_fill_rect1(int argc, VALUE *argv, VALUE self)
  {
    VALUE _x, _y, _width, _height, _color;
    rb_scan_args(argc, argv, "5", &_x, &_y, &_width, &_height, &_color);

    Check_Type(_x, T_FIXNUM);
    Check_Type(_y, T_FIXNUM);
    Check_Type(_width, T_FIXNUM);
    Check_Type(_height, T_FIXNUM);
    Check_Type(_color, T_OBJECT);

    int x = FIX2INT(_x);
    int y = FIX2INT(_y);
    unsigned int width = FIX2INT(_width);
    unsigned int height = FIX2INT(_height);

    Eng::Bitmap *inst = (Eng::Bitmap *)DATA_PTR(self);
    Eng::Color *color = (Eng::Color *)DATA_PTR(_color);

    inst->fill_rect(x, y, width, height, color);

    return self;
  }

  static VALUE overload_fill_rect2(int argc, VALUE *argv, VALUE self)
  {
    VALUE _rect, _color;
    rb_scan_args(argc, argv, "2", &_rect, &_color);

    Check_Type(_rect, T_OBJECT);
    Check_Type(_color, T_OBJECT);

    Eng::Rect *rect = (Eng::Rect *)DATA_PTR(_rect);
    Eng::Color *color = (Eng::Color *)DATA_PTR(_color);

    Eng::Bitmap *inst = (Eng::Bitmap *)DATA_PTR(self);

    inst->fill_rect(rect, color);

    return self;
  }
};

}  // namespace It