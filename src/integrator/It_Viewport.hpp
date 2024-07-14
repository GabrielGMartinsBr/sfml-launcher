#pragma once

#include <stdexcept>

#include "base/Sugars.hpp"
#include "engnine/Engine.hpp"
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

    rb_define_method(viewportClass, "visible", RUBY_METHOD_FUNC(attrGet_visible), 0);
    rb_define_method(viewportClass, "visible=", RUBY_METHOD_FUNC(attrSet_visible), 1);

    rb_define_method(viewportClass, "z", RUBY_METHOD_FUNC(attrGet_z), 0);
    rb_define_method(viewportClass, "z=", RUBY_METHOD_FUNC(attrSet_z), 1);

    rb_define_method(viewportClass, "ox", RUBY_METHOD_FUNC(attrGet_ox), 0);
    rb_define_method(viewportClass, "ox=", RUBY_METHOD_FUNC(attrSet_ox), 1);

    rb_define_method(viewportClass, "oy", RUBY_METHOD_FUNC(attrGet_oy), 0);
    rb_define_method(viewportClass, "oy=", RUBY_METHOD_FUNC(attrSet_oy), 1);
  }

  inline static VALUE getRbClass()
  {
    return rb_const_get(rb_cObject, rb_intern("Viewport"));
  }

  static inline VALUE createRubyObject(Eng::Viewport *instPtr)
  {
    return Data_Wrap_Struct(getRbClass(), 0, free, instPtr);
  }

  static VALUE getRubyObject(Eng::Viewport *viewport)
  {
    if (viewport == nullptr) {
      return Qnil;
    }
    if (viewport->ptr == Qnil) {
      viewport->ptr = createRubyObject(viewport);
    }
    return viewport->ptr;
  }

  static inline Eng::Viewport *getObjectValue(VALUE self)
  {
    return (Eng::Viewport *)DATA_PTR(self);
  }

  static inline bool isInstanceOf(VALUE inst)
  {
    return rb_class_of(inst) == getRbClass();
  }

 private:

  static Eng::Viewport *getInstance(VALUE self)
  {
    return (Eng::Viewport *)DATA_PTR(self);
  }

  /*
    Method initialize
  */

  static VALUE method_initialize(int argc, VALUE *argv, VALUE self)
  {
    if (argc == 4) {
      return overload_initialize1(argc, argv, self);
    } else if (argc == 1) {
      return overload_initialize2(argc, argv, self);
    }

    throw std::runtime_error("Failed to initialize bitmap.");
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

    SharedPtr<Eng::Viewport> inst = MakeSharedPtr<Eng::Viewport>(x, y, width, height);
    Eng::Engine::getInstance().addViewport(inst);

    DATA_PTR(self) = inst.get();

    return self;
  }

  static VALUE overload_initialize2(int argc, VALUE *argv, VALUE self)
  {
    VALUE _rect;
    rb_scan_args(argc, argv, "1", &_rect);

    Check_Type(_rect, T_OBJECT);

    Eng::Rect *rect = (Eng::Rect *)DATA_PTR(_rect);

    SharedPtr<Eng::Viewport> inst = MakeSharedPtr<Eng::Viewport>(rect);
    Eng::Engine::getInstance().addViewport(inst);

    DATA_PTR(self) = inst.get();

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

  /*
   Attr visible
  */

  static VALUE attrGet_visible(VALUE self)
  {
    bool visible = getInstance(self)->getVisible();
    return visible ? Qtrue : Qfalse;
  }

  static VALUE attrSet_visible(VALUE self, VALUE value)
  {
    if (value == Qtrue) {
      getInstance(self)->setVisible(true);
    } else if (value == Qfalse) {
      getInstance(self)->setVisible(false);
    } else {
      throw std::runtime_error("Expected a boolean value.");
    }
    return value;
  }

  /*
   Attr z
  */

  static VALUE attrGet_z(VALUE self)
  {
    int z = getInstance(self)->getZ();
    return INT2FIX(z);
  }

  static VALUE attrSet_z(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    int z = FIX2INT(value);
    getInstance(self)->setZ(z);
    return value;
  }

  /*
   Attr ox
  */

  static VALUE attrGet_ox(VALUE self)
  {
    int ox = getInstance(self)->getOx();
    return INT2FIX(ox);
  }

  static VALUE attrSet_ox(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    int ox = FIX2INT(value);
    getInstance(self)->setOx(ox);
    return value;
  }

  /*
   Attr oy
  */

  static VALUE attrGet_oy(VALUE self)
  {
    int oy = getInstance(self)->getOy();
    return INT2FIX(oy);
  }

  static VALUE attrSet_oy(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    int oy = FIX2INT(value);
    getInstance(self)->setOy(oy);
    return value;
  }
};

}  // namespace It