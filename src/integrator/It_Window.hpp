#pragma once

#include "Convert.hpp"
#include "It_Rect.hpp"
#include "Log.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Window.h"
#include "integrator/It_Bitmap.hpp"
#include "integrator/It_Viewport.hpp"
#include "ruby.h"

// TODO: Implement methods and attributes

namespace It {

class Window {
 public:

  static void integrate()
  {
    VALUE windowClass = rb_define_class("Window", rb_cObject);
    rb_define_alloc_func(windowClass, instance_allocator);

    rb_define_method(windowClass, "initialize", RUBY_METHOD_FUNC(initialize), -1);

    // Attrs

    rb_define_method(windowClass, "windowskin", RUBY_METHOD_FUNC(getter_windowSkin), 0);
    rb_define_method(windowClass, "windowskin=", RUBY_METHOD_FUNC(setter_windowSkin), 1);

    rb_define_method(windowClass, "contents", RUBY_METHOD_FUNC(getter_contents), 0);
    rb_define_method(windowClass, "contents=", RUBY_METHOD_FUNC(setter_contents), 1);

    rb_define_method(windowClass, "stretch", RUBY_METHOD_FUNC(getter_stretch), 0);
    rb_define_method(windowClass, "stretch=", RUBY_METHOD_FUNC(setter_stretch), 1);

    rb_define_method(windowClass, "cursor_rect", RUBY_METHOD_FUNC(getter_cursor_rect), 0);
    rb_define_method(windowClass, "cursor_rect=", RUBY_METHOD_FUNC(setter_cursor_rect), 1);

    rb_define_method(windowClass, "active", RUBY_METHOD_FUNC(getter_active), 0);
    rb_define_method(windowClass, "active=", RUBY_METHOD_FUNC(setter_active), 1);

    rb_define_method(windowClass, "visible", RUBY_METHOD_FUNC(getter_visible), 0);
    rb_define_method(windowClass, "visible=", RUBY_METHOD_FUNC(setter_visible), 1);

    rb_define_method(windowClass, "pause", RUBY_METHOD_FUNC(getter_pause), 0);
    rb_define_method(windowClass, "pause=", RUBY_METHOD_FUNC(setter_pause), 1);

    rb_define_method(windowClass, "x", RUBY_METHOD_FUNC(getter_x), 0);
    rb_define_method(windowClass, "x=", RUBY_METHOD_FUNC(setter_x), 1);

    rb_define_method(windowClass, "y", RUBY_METHOD_FUNC(getter_y), 0);
    rb_define_method(windowClass, "y=", RUBY_METHOD_FUNC(setter_y), 1);

    rb_define_method(windowClass, "width", RUBY_METHOD_FUNC(getter_width), 0);
    rb_define_method(windowClass, "width=", RUBY_METHOD_FUNC(setter_width), 1);

    rb_define_method(windowClass, "height", RUBY_METHOD_FUNC(getter_height), 0);
    rb_define_method(windowClass, "height=", RUBY_METHOD_FUNC(setter_height), 1);

    rb_define_method(windowClass, "z", RUBY_METHOD_FUNC(getter_z), 0);
    rb_define_method(windowClass, "z=", RUBY_METHOD_FUNC(setter_z), 1);

    rb_define_method(windowClass, "ox", RUBY_METHOD_FUNC(getter_ox), 0);
    rb_define_method(windowClass, "ox=", RUBY_METHOD_FUNC(setter_ox), 1);

    rb_define_method(windowClass, "oy", RUBY_METHOD_FUNC(getter_oy), 0);
    rb_define_method(windowClass, "oy=", RUBY_METHOD_FUNC(setter_oy), 1);

    rb_define_method(windowClass, "opacity", RUBY_METHOD_FUNC(getter_opacity), 0);
    rb_define_method(windowClass, "opacity=", RUBY_METHOD_FUNC(setter_opacity), 1);

    rb_define_method(windowClass, "back_opacity", RUBY_METHOD_FUNC(getter_back_opacity), 0);
    rb_define_method(windowClass, "back_opacity=", RUBY_METHOD_FUNC(setter_back_opacity), 1);

    rb_define_method(windowClass, "contents_opacity", RUBY_METHOD_FUNC(getter_contents_opacity), 0);
    rb_define_method(windowClass, "contents_opacity=", RUBY_METHOD_FUNC(setter_contents_opacity), 1);

    // Methods

    rb_define_method(windowClass, "viewport", RUBY_METHOD_FUNC(method_viewport), 0);
    rb_define_method(windowClass, "dispose", RUBY_METHOD_FUNC(method_dispose), 0);
    rb_define_method(windowClass, "disposed?", RUBY_METHOD_FUNC(method_disposed), 0);
    rb_define_method(windowClass, "update", RUBY_METHOD_FUNC(method_update), 0);
  }

  // Utils

  static VALUE getRbClass()
  {
    return rb_const_get(rb_cObject, rb_intern("Window"));
  }

  static VALUE createRubyObject(Eng::Window *inst)
  {
    return Data_Wrap_Struct(getRbClass(), instance_mark, instance_free, inst);
  }

  static VALUE getRubyObject(Eng::Window *inst)
  {
    if (inst == nullptr) {
      return Qnil;
    }
    if (inst->rbObj == Qnil) {
      inst->rbObj = createRubyObject(inst);
    }
    return inst->rbObj;
  }

  static Eng::Window *getObjectValue(VALUE rbObj)
  {
    return (Eng::Window *)DATA_PTR(rbObj);
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
    delete static_cast<Eng::Window *>(ptr);
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
      Eng::Window *inst = new Eng::Window(self);
      DATA_PTR(self) = inst;
      return self;
    }

    if (argc == 1) {
      VALUE _viewport;
      rb_scan_args(argc, argv, "1", &_viewport);
      if (!Viewport::isInstance(_viewport)) {
        RbUtils::raiseCantConvertError(
          rb_class_of(_viewport),
          Viewport::getRbClass()
        );
      }
      Eng::Viewport *viewport = Viewport::getObjectValue(_viewport);
      Eng::Window *inst = new Eng::Window(self, viewport);
      DATA_PTR(self) = inst;
      return self;
    }

    RbUtils::raiseRuntimeException(
      "Window initialize takes 0 or 1 argument, but " + std::to_string(argc) + " were received."
    );
    return Qnil;
  }

  /*
    Get windowSkin
  */

  static VALUE getter_windowSkin(VALUE self)
  {
    Eng::Window *inst = getObjectValue(self);
    return It::Bitmap::getRubyObject(
      inst->getter_windowskin()
    );
  }

  /*
    Set windowSkin
  */

  static VALUE setter_windowSkin(VALUE self, VALUE value)
  {
    Eng::Window *inst = getObjectValue(self);
    Eng::Bitmap *bp = It::Bitmap::getObjectValue(value);
    inst->setter_windowskin(bp);
    return value;
  }

  /*
    Get contents
  */
  static VALUE getter_contents(VALUE self)
  {
    Eng::Window *inst = getObjectValue(self);
    return It::Bitmap::getRubyObject(
      inst->getter_contents()
    );
  }

  /*
    Set contents
  */
  static VALUE setter_contents(VALUE self, VALUE value)
  {
    Eng::Window *inst = getObjectValue(self);
    Eng::Bitmap *bp = It::Bitmap::getObjectValue(value);
    inst->setter_contents(bp);
    return value;
  }

  /*
   Get stretch
  */
  static VALUE getter_stretch(VALUE self)
  {
    Eng::Window *inst = getObjectValue(self);
    return inst->getter_stretch();
  }

  /*
    Set stretch
  */
  static VALUE setter_stretch(VALUE self, VALUE value)
  {
    Eng::Window *inst = getObjectValue(self);
    return inst->setter_stretch(value);
  }

  /*
   Get cursor_rect
  */
  static VALUE getter_cursor_rect(VALUE self)
  {
    Eng::Window *inst = getObjectValue(self);
    Eng::Rect *rect = inst->getter_cursor_rect();
    return It::Rect::getRubyObject(rect);
  }

  /*
    Set cursor_rect
  */
  static VALUE setter_cursor_rect(VALUE self, VALUE value)
  {
    Eng::Rect *rect = Rect::getObjectValue(value);

    Eng::Window *inst = getObjectValue(self);
    inst->setter_cursor_rect(rect);

    return Qnil;
  }

  /*
   Get active
  */
  static VALUE getter_active(VALUE self)
  {
    Eng::Window *inst = getObjectValue(self);
    return inst->getter_active();
  }

  /*
    Set active
  */
  static VALUE setter_active(VALUE self, VALUE value)
  {
    Eng::Window *inst = getObjectValue(self);
    return inst->setter_active(value);
  }

  /*
   Get visible
  */
  static VALUE getter_visible(VALUE self)
  {
    Eng::Window *inst = getObjectValue(self);
    return inst->getter_visible();
  }

  /*
    Set visible
  */
  static VALUE setter_visible(VALUE self, VALUE value)
  {
    Eng::Window *inst = getObjectValue(self);
    return inst->setter_visible(value);
  }

  /*
   Get pause
  */
  static VALUE getter_pause(VALUE self)
  {
    Eng::Window *inst = getObjectValue(self);
    return Convert::toRubyBool(inst->getPause());
  }

  /*
    Set pause
  */
  static VALUE setter_pause(VALUE self, VALUE value)
  {
    int v = Convert::toCBool(value);

    Eng::Window *inst = getObjectValue(self);
    inst->setPause(v);

    return Qnil;
  }

  /*
    Get x
  */
  static VALUE getter_x(VALUE self)
  {
    Eng::Window *inst = getObjectValue(self);
    return Convert::toRubyNumber(inst->getX());
  }

  /*
    Set y
  */
  static VALUE setter_x(VALUE self, VALUE value)
  {
    Eng::Window *inst = getObjectValue(self);
    return inst->setter_x(value);
  }

  /*
    Get y
  */
  static VALUE getter_y(VALUE self)
  {
    Eng::Window *inst = getObjectValue(self);
    return Convert::toRubyNumber(inst->getY());
  }

  /*
    Set y
  */
  static VALUE setter_y(VALUE self, VALUE value)
  {
    Log::out() << "set y: ";
    Eng::Window *inst = getObjectValue(self);
    return inst->setter_y(value);
  }

  /*
    Get width
  */
  static VALUE getter_width(VALUE self)
  {
    Eng::Window *inst = getObjectValue(self);
    return Convert::toRubyNumber(inst->getWidth());
  }

  /*
    Set width
  */
  static VALUE setter_width(VALUE self, VALUE value)
  {
    Eng::Window *inst = getObjectValue(self);
    return inst->setter_width(value);
  }

  /*
    Get height
  */
  static VALUE getter_height(VALUE self)
  {
    Eng::Window *inst = getObjectValue(self);
    return Convert::toRubyNumber(inst->getHeight());
  }

  /*
    Set height
  */
  static VALUE setter_height(VALUE self, VALUE value)
  {
    Eng::Window *inst = getObjectValue(self);
    return inst->setter_height(value);
  }

  /*
   Get z
  */
  static VALUE getter_z(VALUE self)
  {
    Eng::Window *inst = getObjectValue(self);
    return Convert::toRubyNumber(inst->getZ());
  }

  /*
    Set z
  */
  static VALUE setter_z(VALUE self, VALUE value)
  {
    Eng::Window *inst = getObjectValue(self);
    return inst->setter_z(value);
  }

  /*
   Get ox
  */
  static VALUE getter_ox(VALUE self)
  {
    Eng::Window *inst = getObjectValue(self);
    return Convert::toRubyNumber(inst->getter_ox());
  }

  /*
    Set ox
  */
  static VALUE setter_ox(VALUE self, VALUE value)
  {
    Eng::Window *inst = getObjectValue(self);
    return inst->setter_ox(value);
  }

  /*
   Get oy
  */
  static VALUE getter_oy(VALUE self)
  {
    Eng::Window *inst = getObjectValue(self);
    return Convert::toRubyNumber(inst->getter_oy());
  }

  /*
    Set oy
  */
  static VALUE setter_oy(VALUE self, VALUE value)
  {
    Eng::Window *inst = getObjectValue(self);
    return inst->setter_oy(value);
  }

  /*
   Get opacity
  */
  static VALUE getter_opacity(VALUE self)
  {
    Eng::Window *inst = getObjectValue(self);
    return Convert::toRubyNumber(inst->getter_opacity());
  }

  /*
    Set opacity
  */
  static VALUE setter_opacity(VALUE self, VALUE value)
  {
    Eng::Window *inst = getObjectValue(self);
    return inst->setter_opacity(value);
  }

  /*
   Get back_opacity
  */
  static VALUE getter_back_opacity(VALUE self)
  {
    Eng::Window *inst = getObjectValue(self);
    return Convert::toRubyNumber(inst->getter_back_opacity());
  }

  /*
    Set back_opacity
  */
  static VALUE setter_back_opacity(VALUE self, VALUE value)
  {
    Eng::Window *inst = getObjectValue(self);
    return inst->setter_back_opacity(value);
  }

  /*
   Get contents_opacity
  */
  static VALUE getter_contents_opacity(VALUE self)
  {
    Eng::Window *inst = getObjectValue(self);
    return Convert::toRubyNumber(inst->getter_contents_opacity());
  }

  /*
    Set contents_opacity
  */
  static VALUE setter_contents_opacity(VALUE self, VALUE value)
  {
    Eng::Window *inst = getObjectValue(self);
    return inst->setter_contents_opacity(value);
  }

  /*
    Method viewport
  */

  static VALUE method_viewport(VALUE self)
  {
    Eng::Window *inst = getObjectValue(self);
    if (inst == nullptr) {
      return Qnil;
    }

    return It::Viewport::getRubyObject(
      inst->method_viewport()
    );
  }

  /*
    Method dispose
  */

  static VALUE method_dispose(VALUE self)
  {
    Eng::Window *inst = getObjectValue(self);
    if (inst == nullptr) {
      return Qnil;
    }

    inst->method_dispose();

    return Qnil;
  }

  static VALUE method_disposed(VALUE self)
  {
    Eng::Window *inst = getObjectValue(self);
    if (inst == nullptr) {
      return Qnil;
    }

    bool result = inst->method_disposed();

    return result ? Qtrue : Qfalse;
  }

  /*
    Method update
  */

  static VALUE method_update(VALUE self)
  {
    Eng::Window *inst = getObjectValue(self);
    if (inst == nullptr) {
      return Qnil;
    }

    inst->method_update();

    return Qnil;
  }
};
}  // namespace It