#pragma once

#include <stdexcept>

#include "base/Sugars.hpp"
#include "engnine/Engine.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Viewport.hpp"
#include "integrator/It_Color.hpp"
#include "integrator/It_Rect.hpp"
#include "integrator/It_Tone.hpp"
#include "ruby.h"

namespace It {

class Viewport {

 public:

  static void integrate()
  {
    VALUE viewportClass = rb_define_class("Viewport", rb_cObject);

    // Initialize

    rb_define_method(viewportClass, "initialize", RUBY_METHOD_FUNC(method_initialize), -1);

    // Properties

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

    rb_define_method(viewportClass, "color", RUBY_METHOD_FUNC(getter_color), 0);
    rb_define_method(viewportClass, "color=", RUBY_METHOD_FUNC(setter_color), 1);

    rb_define_method(viewportClass, "tone", RUBY_METHOD_FUNC(getter_tone), 0);
    rb_define_method(viewportClass, "tone=", RUBY_METHOD_FUNC(setter_tone), 1);

    // Methods

    rb_define_method(viewportClass, "dispose", RUBY_METHOD_FUNC(method_dispose), 0);
    rb_define_method(viewportClass, "disposed?", RUBY_METHOD_FUNC(method_disposed), 0);
    rb_define_method(viewportClass, "flash", RUBY_METHOD_FUNC(method_flash), 2);
    rb_define_method(viewportClass, "update", RUBY_METHOD_FUNC(method_update), 0);
  }

  // Utils

  static VALUE getRbClass()
  {
    return rb_const_get(rb_cObject, rb_intern("Viewport"));
  }

  static VALUE createRubyObject(Eng::Viewport *inst)
  {
    return Data_Wrap_Struct(getRbClass(), 0, free, inst);
  }

  static VALUE getRubyObject(Eng::Viewport *inst)
  {
    if (inst == nullptr) {
      return Qnil;
    }
    if (inst->ptr == Qnil) {
      inst->ptr = createRubyObject(inst);
    }
    return inst->ptr;
  }

  static Eng::Viewport *getObjectValue(VALUE rbObj)
  {
    return (Eng::Viewport *)DATA_PTR(rbObj);
  }

  static inline bool isInstance(VALUE inst)
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
    inst->ptr = self;

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
    inst->ptr = self;

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

  // Getter color

  static VALUE getter_color(VALUE self)
  {
    Eng::Viewport *inst = getInstance(self);
    Eng::Color *value = inst->getter_color();
    return Color::getRubyObject(value);
  }

  // Setter color

  static VALUE setter_color(VALUE self, VALUE value)
  {
    Eng::Viewport *inst = getInstance(self);
    inst->setter_color(
      Color::getObjectValue(value)
    );
    return value;
  }

  // Getter tone

  static VALUE getter_tone(VALUE self)
  {
    Eng::Viewport *inst = getInstance(self);
    Eng::Tone *value = inst->getter_tone();
    return Tone::getRubyObject(value);
  }

  // Setter tone

  static VALUE setter_tone(VALUE self, VALUE value)
  {
    Eng::Viewport *inst = getInstance(self);
    inst->setter_tone(
      Tone::getObjectValue(value)
    );
    return value;
  }

  /*
    Method dispose
  */

  static VALUE method_dispose(VALUE self)
  {
    Eng::Viewport *inst = getInstance(self);
    inst->method_dispose();
    return Qnil;
  }

  /*
    Method disposed
  */

  static VALUE method_disposed(VALUE self)
  {
    Eng::Viewport *inst = getInstance(self);
    bool isDisposed = inst->method_disposed();
    return isDisposed ? Qtrue : Qfalse;
  }

  /*
    Method flash
  */

  static VALUE method_flash(VALUE self, VALUE _color, VALUE _time)
  {
    if (_color == Qnil || !Color::isInstance(_color)) {
      RbUtils::raiseCantConvertError(
        rb_class_of(_color),
        Color::getRbClass()
      );
      return Qnil;
    }

    int time = Convert::toCInt(_time);
    Eng::Color *color = Color::getObjectValue(_color);

    if (color == nullptr) {
      RbUtils::raiseRuntimeException(
        "Sprite flash method received a null pointer color arg."
      );
      return Qnil;
    }

    Eng::Viewport *inst = getInstance(self);
    inst->method_flash(color, time);

    return Qnil;
  }

  /*
    Method update
  */

  static VALUE method_update(VALUE self)
  {
    Eng::Viewport *inst = getInstance(self);
    inst->method_update();
    return Qnil;
  }
};

}  // namespace It