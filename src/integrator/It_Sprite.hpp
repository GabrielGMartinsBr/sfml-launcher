#pragma once

#include "RbUtils.hpp"
#include "engnine/Color.hpp"
#include "engnine/Sprite.h"
#include "engnine/Viewport.hpp"
#include "integrator/Convert.hpp"
#include "integrator/It_Bitmap.hpp"
#include "integrator/It_Rect.hpp"
#include "integrator/It_Tone.hpp"
#include "integrator/It_Viewport.hpp"
#include "ruby.h"

namespace It {

class Sprite {

 public:
  static void integrate()
  {
    VALUE spriteClass = rb_define_class("Sprite", rb_cObject);
    rb_define_alloc_func(spriteClass, instance_allocator);

    // Initialize

    rb_define_method(spriteClass, "initialize", RUBY_METHOD_FUNC(initialize), -1);

    // Properties

    rb_define_method(spriteClass, "bitmap", RUBY_METHOD_FUNC(getter_bitmap), 0);
    rb_define_method(spriteClass, "bitmap=", RUBY_METHOD_FUNC(setter_bitmap), 1);

    rb_define_method(spriteClass, "src_rect", RUBY_METHOD_FUNC(getter_src_rect), 0);
    rb_define_method(spriteClass, "src_rect=", RUBY_METHOD_FUNC(setter_src_rect), 1);

    rb_define_method(spriteClass, "visible", RUBY_METHOD_FUNC(getter_visible), 0);
    rb_define_method(spriteClass, "visible=", RUBY_METHOD_FUNC(setter_visible), 1);

    rb_define_method(spriteClass, "x", RUBY_METHOD_FUNC(getter_x), 0);
    rb_define_method(spriteClass, "x=", RUBY_METHOD_FUNC(setter_x), 1);

    rb_define_method(spriteClass, "y", RUBY_METHOD_FUNC(getter_y), 0);
    rb_define_method(spriteClass, "y=", RUBY_METHOD_FUNC(setter_y), 1);

    rb_define_method(spriteClass, "z", RUBY_METHOD_FUNC(getter_z), 0);
    rb_define_method(spriteClass, "z=", RUBY_METHOD_FUNC(setter_z), 1);

    rb_define_method(spriteClass, "ox", RUBY_METHOD_FUNC(getter_ox), 0);
    rb_define_method(spriteClass, "ox=", RUBY_METHOD_FUNC(setter_ox), 1);

    rb_define_method(spriteClass, "oy", RUBY_METHOD_FUNC(getter_oy), 0);
    rb_define_method(spriteClass, "oy=", RUBY_METHOD_FUNC(setter_oy), 1);

    rb_define_method(spriteClass, "zoom_x", RUBY_METHOD_FUNC(getter_zoom_x), 0);
    rb_define_method(spriteClass, "zoom_x=", RUBY_METHOD_FUNC(setter_zoom_x), 1);

    rb_define_method(spriteClass, "zoom_y", RUBY_METHOD_FUNC(getter_zoom_y), 0);
    rb_define_method(spriteClass, "zoom_y=", RUBY_METHOD_FUNC(setter_zoom_y), 1);

    rb_define_method(spriteClass, "angle", RUBY_METHOD_FUNC(getter_angle), 0);
    rb_define_method(spriteClass, "angle=", RUBY_METHOD_FUNC(setter_angle), 1);

    rb_define_method(spriteClass, "mirror", RUBY_METHOD_FUNC(getter_mirror), 0);
    rb_define_method(spriteClass, "mirror=", RUBY_METHOD_FUNC(setter_mirror), 1);

    rb_define_method(spriteClass, "bush_depth", RUBY_METHOD_FUNC(getter_bush_depth), 0);
    rb_define_method(spriteClass, "bush_depth=", RUBY_METHOD_FUNC(setter_bush_depth), 1);

    rb_define_method(spriteClass, "opacity", RUBY_METHOD_FUNC(getter_opacity), 0);
    rb_define_method(spriteClass, "opacity=", RUBY_METHOD_FUNC(setter_opacity), 1);

    rb_define_method(spriteClass, "blend_type", RUBY_METHOD_FUNC(getter_blend_type), 0);
    rb_define_method(spriteClass, "blend_type=", RUBY_METHOD_FUNC(setter_blend_type), 1);

    rb_define_method(spriteClass, "color", RUBY_METHOD_FUNC(getter_color), 0);
    rb_define_method(spriteClass, "color=", RUBY_METHOD_FUNC(setter_color), 1);

    rb_define_method(spriteClass, "tone", RUBY_METHOD_FUNC(getter_tone), 0);
    rb_define_method(spriteClass, "tone=", RUBY_METHOD_FUNC(setter_tone), 1);

    // Methods

    rb_define_method(spriteClass, "viewport", RUBY_METHOD_FUNC(method_viewport), 0);
    rb_define_method(spriteClass, "dispose", RUBY_METHOD_FUNC(method_dispose), 0);
    rb_define_method(spriteClass, "disposed?", RUBY_METHOD_FUNC(method_disposed), 0);
    rb_define_method(spriteClass, "flash", RUBY_METHOD_FUNC(method_flash), 2);
    rb_define_method(spriteClass, "update", RUBY_METHOD_FUNC(method_update), 0);
  }

  // Utils

  static VALUE getRbClass()
  {
    return rb_const_get(rb_cObject, rb_intern("Sprite"));
  }

  static VALUE createRubyObject(Eng::Sprite *inst)
  {
    return Data_Wrap_Struct(getRbClass(), instance_mark, instance_free, inst);
  }

  static VALUE getRubyObject(Eng::Sprite *inst)
  {
    if (inst == nullptr) {
      return Qnil;
    }
    if (inst->rbObj == Qnil) {
      inst->rbObj = createRubyObject(inst);
    }
    return inst->rbObj;
  }

  static Eng::Sprite *getObjectValue(VALUE rbObj)
  {
    return (Eng::Sprite *)DATA_PTR(rbObj);
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
    delete static_cast<Eng::Sprite *>(ptr);
  }

  static void instance_mark(void *ptr)
  {
  }

  /*
    Method initialize
  */

  static VALUE initialize(int argc, VALUE *argv, VALUE self)
  {
    Eng::Sprite *instance;

    if (argc == 0) {
      instance = new Eng::Sprite(self);
      DATA_PTR(self) = instance;
      return self;
    }

    if (argc == 1) {
      VALUE _viewport;
      rb_scan_args(argc, argv, "1", &_viewport);
      if (_viewport != Qnil && !Viewport::isInstance(_viewport)) {
        RbUtils::raiseCantConvertError(
          rb_class_of(_viewport),
          Viewport::getRbClass()
        );
      }
      Eng::Viewport *viewport = nullptr;
      if (_viewport != Qnil) {
        viewport = Viewport::getObjectValue(_viewport);
      }

      instance = new Eng::Sprite(self, viewport);
      DATA_PTR(self) = instance;
      return self;
    }

    RbUtils::raiseRuntimeException(
      "Sprite initialize takes 0 or 1 argument, but " + std::to_string(argc) + " were received."
    );
    return Qnil;
  }

  // Getter bitmap

  static VALUE getter_bitmap(VALUE self)
  {
    Eng::Sprite *inst = getObjectValue(self);
    return Bitmap::getRubyObject(
      inst->getter_bitmap()
    );
  }

  // Setter bitmap

  static VALUE setter_bitmap(VALUE self, VALUE value)
  {
    if (value != Qnil && !Bitmap::isInstanceFrom(value)) {
      RbUtils::raiseCantConvertError(
        rb_class_of(value),
        Bitmap::getRbClass()
      );
      return Qnil;
    }

    Eng::Sprite *inst = getObjectValue(self);
    inst->setter_bitmap(
      Bitmap::getObjectValue(value)
    );

    return value;
  }

  // Getter src_rect

  static VALUE getter_src_rect(VALUE self)
  {
    Eng::Sprite *inst = getObjectValue(self);
    return Rect::getRubyObject(
      inst->getter_src_rect()
    );
  }

  // Setter src_rect

  static VALUE setter_src_rect(VALUE self, VALUE value)
  {
    if (value != Qnil && !Rect::isInstance(value)) {
      RbUtils::raiseCantConvertError(
        rb_class_of(value),
        Rect::getRbClass()
      );
      return Qnil;
    }

    Eng::Sprite *inst = getObjectValue(self);
    inst->setInstanceVar("@src_rect", value);
    inst->setter_src_rect(
      Rect::getObjectValue(value)
    );

    return value;
  }

  // Getter visible

  static VALUE getter_visible(VALUE self)
  {
    Eng::Sprite *inst = getObjectValue(self);
    return Convert::toRubyBool(
      inst->getter_visible()
    );
  }

  // Setter visible

  static VALUE setter_visible(VALUE self, VALUE value)
  {
    Eng::Sprite *inst = getObjectValue(self);
    inst->setter_visible(
      Convert::toCBool(value)
    );
    return value;
  }

  // Getter x

  static VALUE getter_x(VALUE self)
  {
    Eng::Sprite *inst = getObjectValue(self);
    return Convert::toRubyNumber(
      inst->getter_x()
    );
  }

  // Setter x

  static VALUE setter_x(VALUE self, VALUE value)
  {
    Eng::Sprite *inst = getObjectValue(self);
    inst->setter_x(
      Convert::toCInt2(value)
    );
    return value;
  }

  // Getter y

  static VALUE getter_y(VALUE self)
  {
    Eng::Sprite *inst = getObjectValue(self);
    return Convert::toRubyNumber(
      inst->getter_y()
    );
  }

  // Setter y

  static VALUE setter_y(VALUE self, VALUE value)
  {
    Eng::Sprite *inst = getObjectValue(self);
    inst->setter_y(
      Convert::toCInt2(value)
    );
    return value;
  }

  // Getter z

  static VALUE getter_z(VALUE self)
  {
    Eng::Sprite *inst = getObjectValue(self);
    return Convert::toRubyNumber(
      inst->getter_z()
    );
  }

  // Setter z

  static VALUE setter_z(VALUE self, VALUE value)
  {
    Eng::Sprite *inst = getObjectValue(self);
    inst->setter_z(
      Convert::toCInt(value)
    );
    return value;
  }

  // Getter ox

  static VALUE getter_ox(VALUE self)
  {
    Eng::Sprite *inst = getObjectValue(self);
    return Convert::toRubyNumber(
      inst->getter_ox()
    );
  }

  // Setter ox

  static VALUE setter_ox(VALUE self, VALUE value)
  {
    Eng::Sprite *inst = getObjectValue(self);
    inst->setter_ox(
      Convert::toCInt(value)
    );
    return value;
  }

  // Getter oy

  static VALUE getter_oy(VALUE self)
  {
    Eng::Sprite *inst = getObjectValue(self);
    return Convert::toRubyNumber(
      inst->getter_oy()
    );
  }

  // Setter oy

  static VALUE setter_oy(VALUE self, VALUE value)
  {
    Eng::Sprite *inst = getObjectValue(self);
    inst->setter_oy(
      Convert::toCInt(value)
    );
    return value;
  }

  // Getter zoom_x

  static VALUE getter_zoom_x(VALUE self)
  {
    Eng::Sprite *inst = getObjectValue(self);
    return Convert::toRubyDouble(
      inst->getter_zoom_x()
    );
  }

  // Setter zoom_x

  static VALUE setter_zoom_x(VALUE self, VALUE value)
  {
    Eng::Sprite *inst = getObjectValue(self);
    inst->setter_zoom_x(
      Convert::toCDouble(value)
    );
    return value;
  }

  // Getter zoom_y

  static VALUE getter_zoom_y(VALUE self)
  {
    Eng::Sprite *inst = getObjectValue(self);
    return Convert::toRubyDouble(
      inst->getter_zoom_y()
    );
  }

  // Setter zoom_y

  static VALUE setter_zoom_y(VALUE self, VALUE value)
  {
    Eng::Sprite *inst = getObjectValue(self);
    inst->setter_zoom_y(
      Convert::toCDouble(value)
    );
    return value;
  }

  // Getter angle

  static VALUE getter_angle(VALUE self)
  {
    Eng::Sprite *inst = getObjectValue(self);
    return Convert::toRubyDouble(
      inst->getter_angle()
    );
  }

  // Setter angle

  static VALUE setter_angle(VALUE self, VALUE value)
  {
    Eng::Sprite *inst = getObjectValue(self);
    inst->setter_angle(
      Convert::toCDouble2(value)
    );
    return value;
  }

  // Getter mirror

  static VALUE getter_mirror(VALUE self)
  {
    Eng::Sprite *inst = getObjectValue(self);
    return Convert::toRubyBool(
      inst->getter_mirror()
    );
  }

  // Setter mirror

  static VALUE setter_mirror(VALUE self, VALUE value)
  {
    Eng::Sprite *inst = getObjectValue(self);
    inst->setter_mirror(
      Convert::toCBool(value)
    );
    return value;
  }

  // Getter bush_depth

  static VALUE getter_bush_depth(VALUE self)
  {
    Eng::Sprite *inst = getObjectValue(self);
    return Convert::toRubyNumber(
      inst->getter_bush_depth()
    );
  }

  // Setter bush_depth

  static VALUE setter_bush_depth(VALUE self, VALUE value)
  {
    Eng::Sprite *inst = getObjectValue(self);
    inst->setter_bush_depth(
      Convert::toCInt(value)
    );
    return value;
  }

  // Getter opacity

  static VALUE getter_opacity(VALUE self)
  {
    Eng::Sprite *inst = getObjectValue(self);
    return Convert::toRubyNumber(
      inst->getter_opacity()
    );
  }

  // Setter opacity

  static VALUE setter_opacity(VALUE self, VALUE value)
  {
    Eng::Sprite *inst = getObjectValue(self);
    inst->setter_opacity(
      Convert::toCInt2(value)
    );
    return value;
  }

  // Getter blend_type

  static VALUE setter_blend_type(VALUE self, VALUE value)
  {
    Eng::Sprite *inst = getObjectValue(self);
    inst->setter_blend_type(
      Convert::toCInt(value)
    );
    return value;
  }

  // Setter blend_type

  static VALUE getter_blend_type(VALUE self)
  {
    Eng::Sprite *inst = getObjectValue(self);
    return Convert::toRubyNumber(
      inst->getter_blend_type()
    );
  }

  // Getter color

  static VALUE getter_color(VALUE self)
  {
    Eng::Sprite *inst = getObjectValue(self);
    Eng::Color *value = inst->getter_color();
    return Color::getRubyObject(value);
  }

  // Setter color

  static VALUE setter_color(VALUE self, VALUE value)
  {
    Eng::Sprite *inst = getObjectValue(self);
    inst->setter_color(
      Color::getObjectValue(value)
    );
    return value;
  }

  // Getter tone

  static VALUE getter_tone(VALUE self)
  {
    Eng::Sprite *inst = getObjectValue(self);
    Eng::Tone *value = inst->getter_tone();
    return Tone::getRubyObject(value);
  }

  // Setter tone

  static VALUE setter_tone(VALUE self, VALUE value)
  {
    Eng::Sprite *inst = getObjectValue(self);
    inst->setter_tone(
      Tone::getObjectValue(value)
    );
    return value;
  }

  /*
    Method viewport
  */

  static VALUE method_viewport(VALUE self)
  {
    Eng::Sprite *inst = getObjectValue(self);
    return Viewport::getRubyObject(
      inst->method_viewport()
    );
  }

  /*
    Method dispose
  */

  static VALUE method_dispose(VALUE self)
  {
    Eng::Sprite *inst = (Eng::Sprite *)DATA_PTR(self);
    inst->method_dispose();
    return Qnil;
  }

  /*
    Method disposed
  */

  static VALUE method_disposed(VALUE self)
  {
    Eng::Sprite *inst = (Eng::Sprite *)DATA_PTR(self);
    bool isDisposed = inst->method_disposed();
    return isDisposed ? Qtrue : Qfalse;
  }

  /*
    Method flash
  */

  static VALUE method_flash(VALUE self, VALUE _color, VALUE _time)
  {
    if (_color != Qnil && !Color::isInstance(_color)) {
      RbUtils::raiseCantConvertError(
        rb_class_of(_color),
        Color::getRbClass()
      );
      return Qnil;
    }

    Eng::Color *color = nullptr;
    if (_color != Qnil) {
      color = Color::getObjectValue(_color);
    }

    int time = Convert::toCInt(_time);

    Eng::Sprite *inst = getObjectValue(self);
    inst->method_flash(color, time);

    return Qnil;
  }

  /*
    Method update
  */

  static VALUE method_update(VALUE self)
  {
    Eng::Sprite *inst = getObjectValue(self);
    inst->method_update();
    return Qnil;
  }
};

}  // namespace It