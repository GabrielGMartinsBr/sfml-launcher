#pragma once

#include "RbUtils.hpp"
#include "engnine/Bitmap.h"
#include "engnine/Color.hpp"
#include "engnine/Plane.hpp"
#include "engnine/Tone.hpp"
#include "engnine/Viewport.hpp"
#include "integrator/Convert.hpp"
#include "integrator/It_Bitmap.hpp"
#include "integrator/It_Color.hpp"
#include "integrator/It_Tone.hpp"
#include "integrator/It_Viewport.hpp"
#include "ruby.h"

namespace It {

class Plane {
 public:

  static void integrate()
  {
    VALUE planeClass = rb_define_class("Plane", rb_cObject);
    rb_define_alloc_func(planeClass, instance_allocator);

    // Initialize

    rb_define_method(planeClass, "initialize", RUBY_METHOD_FUNC(initialize), -1);

    // Properties

    rb_define_method(planeClass, "bitmap", RUBY_METHOD_FUNC(getter_bitmap), 0);
    rb_define_method(planeClass, "bitmap=", RUBY_METHOD_FUNC(setter_bitmap), 1);

    rb_define_method(planeClass, "visible", RUBY_METHOD_FUNC(getter_visible), 0);
    rb_define_method(planeClass, "visible=", RUBY_METHOD_FUNC(setter_visible), 1);

    rb_define_method(planeClass, "z", RUBY_METHOD_FUNC(getter_z), 0);
    rb_define_method(planeClass, "z=", RUBY_METHOD_FUNC(setter_z), 1);

    rb_define_method(planeClass, "ox", RUBY_METHOD_FUNC(getter_ox), 0);
    rb_define_method(planeClass, "ox=", RUBY_METHOD_FUNC(setter_ox), 1);

    rb_define_method(planeClass, "oy", RUBY_METHOD_FUNC(getter_oy), 0);
    rb_define_method(planeClass, "oy=", RUBY_METHOD_FUNC(setter_oy), 1);

    rb_define_method(planeClass, "zoom_x", RUBY_METHOD_FUNC(getter_zoom_x), 0);
    rb_define_method(planeClass, "zoom_x=", RUBY_METHOD_FUNC(setter_zoom_x), 1);

    rb_define_method(planeClass, "zoom_y", RUBY_METHOD_FUNC(getter_zoom_y), 0);
    rb_define_method(planeClass, "zoom_y=", RUBY_METHOD_FUNC(setter_zoom_y), 1);

    rb_define_method(planeClass, "opacity", RUBY_METHOD_FUNC(getter_opacity), 0);
    rb_define_method(planeClass, "opacity=", RUBY_METHOD_FUNC(setter_opacity), 1);

    rb_define_method(planeClass, "blend_type", RUBY_METHOD_FUNC(getter_blend_type), 0);
    rb_define_method(planeClass, "blend_type=", RUBY_METHOD_FUNC(setter_blend_type), 1);

    rb_define_method(planeClass, "color", RUBY_METHOD_FUNC(getter_color), 0);
    rb_define_method(planeClass, "color=", RUBY_METHOD_FUNC(setter_color), 1);

    rb_define_method(planeClass, "tone", RUBY_METHOD_FUNC(getter_tone), 0);
    rb_define_method(planeClass, "tone=", RUBY_METHOD_FUNC(setter_tone), 1);

    // Methods

    rb_define_method(planeClass, "dispose", RUBY_METHOD_FUNC(method_dispose), 0);
    rb_define_method(planeClass, "disposed?", RUBY_METHOD_FUNC(method_disposed), 0);
    rb_define_method(planeClass, "viewport", RUBY_METHOD_FUNC(method_viewport), 0);
  }

  // Utils

  static VALUE getRbClass()
  {
    return rb_const_get(rb_cObject, rb_intern("Plane"));
  }

  static VALUE createRubyObject(Eng::Plane *inst)
  {
    return Data_Wrap_Struct(getRbClass(), instance_mark, instance_free, inst);
  }

  static VALUE getRubyObject(Eng::Plane *inst)
  {
    if (inst == nullptr) {
      return Qnil;
    }
    if (inst->rbObj == Qnil) {
      inst->rbObj = createRubyObject(inst);
    }
    return inst->rbObj;
  }

  static Eng::Plane *getObjectValue(VALUE rbObj)
  {
    return (Eng::Plane *)DATA_PTR(rbObj);
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
    Log::out() << "[[Plane_free]]";
    delete static_cast<Eng::Plane *>(ptr);
  }

  static void instance_mark(void *ptr)
  {
  }

  /*
    Method initialize
  */

  static VALUE initialize(int argc, VALUE *argv, VALUE self)
  {
    if (argc == 0) {
      Eng::Plane *inst = new Eng::Plane(self);
      DATA_PTR(self) = inst;
      return self;
    }

    if (argc == 1) {
      VALUE _viewport;
      rb_scan_args(argc, argv, "1", &_viewport);
      Eng::Viewport *viewport = Viewport::getObjectValue(_viewport);
      Eng::Plane *inst = new Eng::Plane(self, viewport);
      DATA_PTR(self) = inst;
      return self;
    }

    RbUtils::raiseRuntimeException(
      "Plane initializer takes 1 or 0 argument, but " + std::to_string(argc) + " were received."
    );
    return Qnil;
  }

  /*
      Properties
  */

  // Getter bitmap

  static VALUE getter_bitmap(VALUE self)
  {
    Eng::Plane *inst = getObjectValue(self);
    Eng::Bitmap *bitmap = inst->getter_bitmap();
    if (bitmap == nullptr) {
      return Qnil;
    }
    return Bitmap::getRubyObject(bitmap);
  }

  // Setter bitmap

  static VALUE setter_bitmap(VALUE self, VALUE value)
  {
    Eng::Plane *inst = getObjectValue(self);
    inst->setter_bitmap(
      Bitmap::getObjectValue(value)
    );
    return value;
  }

  // Getter visible

  static VALUE getter_visible(VALUE self)
  {
    Eng::Plane *inst = getObjectValue(self);
    return Convert::toRubyBool(
      inst->getter_visible()
    );
  }

  // Setter visible

  static VALUE setter_visible(VALUE self, VALUE value)
  {
    Eng::Plane *inst = getObjectValue(self);
    inst->setter_visible(
      Convert::toCBool(value)
    );
    return value;
  }

  // Getter z

  static VALUE getter_z(VALUE self)
  {
    Eng::Plane *inst = getObjectValue(self);
    return Convert::toRubyNumber(
      inst->getter_z()
    );
  }

  // Setter z

  static VALUE setter_z(VALUE self, VALUE value)
  {
    Eng::Plane *inst = getObjectValue(self);
    inst->setter_z(
      Convert::toCInt(value)
    );
    return value;
  }

  // Getter ox

  static VALUE getter_ox(VALUE self)
  {
    Eng::Plane *inst = getObjectValue(self);
    return Convert::toRubyNumber(
      inst->getter_ox()
    );
  }

  // Setter ox

  static VALUE setter_ox(VALUE self, VALUE value)
  {
    Eng::Plane *inst = getObjectValue(self);
    inst->setter_ox(
      Convert::toCInt2(value)
    );
    return value;
  }

  // Getter oy

  static VALUE getter_oy(VALUE self)
  {
    Eng::Plane *inst = getObjectValue(self);
    return Convert::toRubyNumber(
      inst->getter_oy()
    );
  }

  // Setter oy

  static VALUE setter_oy(VALUE self, VALUE value)
  {
    Eng::Plane *inst = getObjectValue(self);
    inst->setter_oy(
      Convert::toCInt2(value)
    );
    return value;
  }

  // Getter zoom_x

  static VALUE getter_zoom_x(VALUE self)
  {
    Eng::Plane *inst = getObjectValue(self);
    return Convert::toRubyDouble(
      inst->getter_zoom_x()
    );
  }

  // Setter zoom_x

  static VALUE setter_zoom_x(VALUE self, VALUE value)
  {
    Eng::Plane *inst = getObjectValue(self);
    inst->setter_zoom_x(
      Convert::toCDouble(value)
    );
    return value;
  }

  // Getter zoom_y

  static VALUE getter_zoom_y(VALUE self)
  {
    Eng::Plane *inst = getObjectValue(self);
    return Convert::toRubyDouble(
      inst->getter_zoom_y()
    );
  }

  // Setter zoom_y

  static VALUE setter_zoom_y(VALUE self, VALUE value)
  {
    Eng::Plane *inst = getObjectValue(self);
    inst->setter_zoom_y(
      Convert::toCDouble(value)
    );
    return value;
  }

  // Getter opacity

  static VALUE getter_opacity(VALUE self)
  {
    Eng::Plane *inst = getObjectValue(self);
    return Convert::toRubyNumber(
      inst->getter_opacity()
    );
  }

  // Setter opacity

  static VALUE setter_opacity(VALUE self, VALUE value)
  {
    Eng::Plane *inst = getObjectValue(self);
    inst->setter_opacity(
      Convert::toCInt(value)
    );
    return value;
  }

  // Getter blend_type

  static VALUE getter_blend_type(VALUE self)
  {
    Eng::Plane *inst = getObjectValue(self);
    return Convert::toRubyNumber(
      inst->getter_blend_type()
    );
  }

  // Setter blend_type

  static VALUE setter_blend_type(VALUE self, VALUE value)
  {
    Eng::Plane *inst = getObjectValue(self);
    inst->setter_blend_type(
      Convert::toCInt(value)
    );
    return value;
  }

  // Getter color

  static VALUE getter_color(VALUE self)
  {
    Eng::Plane *inst = getObjectValue(self);
    Eng::Color *value = inst->getter_color();
    return Color::getRubyObject(value);
  }

  // Setter color

  static VALUE setter_color(VALUE self, VALUE value)
  {
    Eng::Plane *inst = getObjectValue(self);
    inst->setter_color(
      Color::getObjectValue(value)
    );
    return value;
  }

  // Getter tone

  static VALUE getter_tone(VALUE self)
  {
    Eng::Plane *inst = getObjectValue(self);
    Eng::Tone *value = inst->getter_tone();
    if (value == nullptr) {
      return Qnil;
    }
    return Tone::getRubyObject(value);
  }

  // Setter tone

  static VALUE setter_tone(VALUE self, VALUE value)
  {
    Eng::Plane *inst = getObjectValue(self);
    inst->setter_tone(
      Tone::getObjectValue(value)
    );
    return value;
  }

  /*
    Methods
  */

  static VALUE method_dispose(VALUE self)
  {
    Eng::Plane *inst = getObjectValue(self);
    inst->method_dispose();
    return Qnil;
  }

  static VALUE method_disposed(VALUE self)
  {
    Eng::Plane *inst = getObjectValue(self);
    return Convert::toRubyBool(
      inst->method_disposed()
    );
  }

  static VALUE method_viewport(VALUE self)
  {
    Eng::Plane *inst = getObjectValue(self);
    return Viewport::getRubyObject(
      inst->method_viewport()
    );
  }
};

}  // namespace It