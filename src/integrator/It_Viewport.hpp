#pragma once

#include <stdexcept>

#include "RbUtils.hpp"
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
    rb_define_alloc_func(viewportClass, instance_allocator);

    // Initialize

    rb_define_method(viewportClass, "initialize", RUBY_METHOD_FUNC(method_initialize), -1);

    // Properties

    rb_define_method(viewportClass, "rect", RUBY_METHOD_FUNC(getter_rect), 0);
    rb_define_method(viewportClass, "rect=", RUBY_METHOD_FUNC(setter_rect), 1);

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
    return Data_Wrap_Struct(getRbClass(), instance_mark, instance_free, inst);
  }

  static VALUE getRubyObject(Eng::Viewport *inst)
  {
    if (inst == nullptr) {
      return Qnil;
    }
    if (inst->rbObj == Qnil) {
      inst->rbObj = createRubyObject(inst);
    }
    return inst->rbObj;
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

  /*
    Allocator
  */

  static VALUE instance_allocator(VALUE instanceClass)
  {
    return Data_Wrap_Struct(instanceClass, instance_mark, instance_free, nullptr);
  }

  /*
    Deallocator
  */

  static void instance_free(void *ptr)
  {
    delete static_cast<Eng::Viewport *>(ptr);
  }

  static void instance_mark(void *ptr)
  {
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

    int x = Convert::toCInt2(_x);
    int y = Convert::toCInt2(_y);
    int width = Convert::toCInt2(_width);
    int height = Convert::toCInt2(_height);

    // SharedPtr<Eng::Viewport> inst = MakeSharedPtr<Eng::Viewport>(x, y, width, height);
    // Eng::Engine::getInstance().addViewport(inst);
    // DATA_PTR(self) = inst.get();

    Eng::Viewport *inst = new Eng::Viewport(self, x, y, width, height);
    DATA_PTR(self) = inst;

    return self;
  }

  static VALUE overload_initialize2(int argc, VALUE *argv, VALUE self)
  {
    VALUE _rect;
    rb_scan_args(argc, argv, "1", &_rect);

    Eng::Rect *rect = Rect::getObjectValue(_rect);

    // SharedPtr<Eng::Viewport> inst = MakeSharedPtr<Eng::Viewport>(rect);
    // Eng::Engine::getInstance().addViewport(inst);
    // DATA_PTR(self) = inst.get();

    Eng::Viewport *inst = new Eng::Viewport(self, rect);
    DATA_PTR(self) = inst;

    return self;
  }

  /*
    Attr rect
  */

  static VALUE getter_rect(VALUE self)
  {
    Eng::Viewport *inst = getObjectValue(self);
    Eng::Rect *rect = inst->getRect();
    return Rect::getRubyObject(rect);
  }

  static VALUE setter_rect(VALUE self, VALUE value)
  {
    Eng::Rect *rect = Rect::getObjectValue(value);
    if (rect == nullptr) {
      RbUtils::raiseRuntimeException("Invalid rect argument received.");
      return Qnil;
    }

    Eng::Viewport *inst = getObjectValue(self);
    inst->setRect(rect);

    return value;
  }

  /*
   Attr visible
  */

  static VALUE attrGet_visible(VALUE self)
  {
    bool visible = getObjectValue(self)->getVisible();
    return visible ? Qtrue : Qfalse;
  }

  static VALUE attrSet_visible(VALUE self, VALUE value)
  {
    if (value == Qtrue) {
      getObjectValue(self)->setVisible(true);
    } else if (value == Qfalse) {
      getObjectValue(self)->setVisible(false);
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
    int z = getObjectValue(self)->getZ();
    return INT2FIX(z);
  }

  static VALUE attrSet_z(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    int z = FIX2INT(value);
    getObjectValue(self)->setZ(z);
    return value;
  }

  /*
   Attr ox
  */

  static VALUE attrGet_ox(VALUE self)
  {
    int ox = getObjectValue(self)->getOx();
    return INT2FIX(ox);
  }

  static VALUE attrSet_ox(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    int ox = FIX2INT(value);
    getObjectValue(self)->setOx(ox);
    return value;
  }

  /*
   Attr oy
  */

  static VALUE attrGet_oy(VALUE self)
  {
    int oy = getObjectValue(self)->getOy();
    return INT2FIX(oy);
  }

  static VALUE attrSet_oy(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    int oy = FIX2INT(value);
    getObjectValue(self)->setOy(oy);
    return value;
  }

  // Getter color

  static VALUE getter_color(VALUE self)
  {
    Eng::Viewport *inst = getObjectValue(self);
    Eng::Color *value = inst->getter_color();
    return Color::getRubyObject(value);
  }

  // Setter color

  static VALUE setter_color(VALUE self, VALUE value)
  {
    Eng::Viewport *inst = getObjectValue(self);
    inst->setter_color(
      Color::getObjectValue(value)
    );
    return value;
  }

  // Getter tone

  static VALUE getter_tone(VALUE self)
  {
    Eng::Viewport *inst = getObjectValue(self);
    Eng::Tone *value = inst->getter_tone();
    return Tone::getRubyObject(value);
  }

  // Setter tone

  static VALUE setter_tone(VALUE self, VALUE value)
  {
    Eng::Viewport *inst = getObjectValue(self);
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
    Eng::Viewport *inst = getObjectValue(self);
    inst->method_dispose();
    return Qnil;
  }

  /*
    Method disposed
  */

  static VALUE method_disposed(VALUE self)
  {
    Eng::Viewport *inst = getObjectValue(self);
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

    Eng::Viewport *inst = getObjectValue(self);
    inst->method_flash(color, time);

    return Qnil;
  }

  /*
    Method update
  */

  static VALUE method_update(VALUE self)
  {
    Eng::Viewport *inst = getObjectValue(self);
    inst->method_update();
    return Qnil;
  }
};

}  // namespace It