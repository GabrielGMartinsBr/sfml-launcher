#pragma once

#include "Convert.hpp"
#include "It_Rect.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Window.hpp"
#include "ruby.h"

// TODO: Implement methods and attributes

namespace It {

class Window {
 public:

  static void integrate()
  {
    VALUE windowClass = rb_define_class("Window", rb_cObject);

    rb_define_method(windowClass, "initialize", RUBY_METHOD_FUNC(method_initialize), -1);
    rb_define_method(windowClass, "update", RUBY_METHOD_FUNC(method_update), 0);

    rb_define_method(windowClass, "update", RUBY_METHOD_FUNC(method_update), 0);

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

    rb_define_method(windowClass, "dispose", RUBY_METHOD_FUNC(method_dispose), 0);
    rb_define_method(windowClass, "disposed?", RUBY_METHOD_FUNC(method_dispose), 0);
  }

 private:
  static VALUE method_initialize(int argc, VALUE *argv, VALUE self)
  {
    Eng::Window *window = new Eng::Window();

    DATA_PTR(self) = window;

    return self;
  }

  static VALUE method_update(VALUE self)
  {
    return Qnil;
  }

  /*
      Get windowSkin
  */

  static VALUE getter_windowSkin(VALUE self)
  {
    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    auto v = inst->getWindowSkin();

    Check_Type(v, T_OBJECT);

    if (v == nullptr) {
      return Qnil;
    }

    return v->ptr;
  }

  /*
      Set windowSkin
  */

  static VALUE setter_windowSkin(VALUE self, VALUE value)
  {
    Check_Type(value, T_OBJECT);

    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    Eng::Bitmap *bp = (Eng::Bitmap *)DATA_PTR(value);

    inst->setWindowSkin(bp);

    return Qnil;
  }

  /*
    Get contents
  */
  static VALUE getter_contents(VALUE self)
  {
    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    Eng::Bitmap *bp = inst->getContents();

    if (bp == nullptr) {
      return Qnil;
    }

    return bp->ptr;
  }

  /*
    Set contents
  */
  static VALUE setter_contents(VALUE self, VALUE value)
  {
    Check_Type(value, T_OBJECT);
    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    Eng::Bitmap *bp = (Eng::Bitmap *)DATA_PTR(value);

    inst->setContents(bp);

    return Qnil;
  }

  /*
   Get stretch
  */
  static VALUE getter_stretch(VALUE self)
  {
    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    return Convert::toRubyBool(inst->getter_stretch());
  }

  /*
    Set stretch
  */
  static VALUE setter_stretch(VALUE self, VALUE value)
  {
    int v = Convert::toCBool(value);

    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    inst->setter_stretch(v);

    return Qnil;
  }

  /*
   Get cursor_rect
  */
  static VALUE getter_cursor_rect(VALUE self)
  {
    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    Eng::Rect *rect = inst->getter_cursor_rect();
    return It::Rect::getRubyObject(rect);
  }

  /*
    Set cursor_rect
  */
  static VALUE setter_cursor_rect(VALUE self, VALUE value)
  {
    Eng::Rect *rect = (Eng::Rect *)DATA_PTR(value);

    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    inst->setter_cursor_rect(rect);

    return Qnil;
  }

  /*
   Get active
  */
  static VALUE getter_active(VALUE self)
  {
    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    return Convert::toRubyBool(inst->getter_active());
  }

  /*
    Set active
  */
  static VALUE setter_active(VALUE self, VALUE value)
  {
    int v = Convert::toCBool(value);

    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    inst->setter_active(v);

    return Qnil;
  }

  /*
   Get visible
  */
  static VALUE getter_visible(VALUE self)
  {
    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    return Convert::toRubyBool(inst->getter_visible());
  }

  /*
    Set visible
  */
  static VALUE setter_visible(VALUE self, VALUE value)
  {
    int v = Convert::toCBool(value);

    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    inst->setter_visible(v);

    return Qnil;
  }

  /*
   Get pause
  */
  static VALUE getter_pause(VALUE self)
  {
    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    return Convert::toRubyBool(inst->getter_pause());
  }

  /*
    Set pause
  */
  static VALUE setter_pause(VALUE self, VALUE value)
  {
    int v = Convert::toCBool(value);

    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    inst->setter_pause(v);

    return Qnil;
  }

  /*
    Get x
  */
  static VALUE getter_x(VALUE self)
  {
    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    return Convert::toRubyNumber(inst->getX());
  }

  /*
    Set y
  */
  static VALUE setter_x(VALUE self, VALUE value)
  {
    int x = Convert::toCInt(value);

    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    inst->setX(x);

    return Qnil;
  }

  /*
    Get y
  */
  static VALUE getter_y(VALUE self)
  {
    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    return Convert::toRubyNumber(inst->getY());
  }

  /*
    Set y
  */
  static VALUE setter_y(VALUE self, VALUE value)
  {
    int y = Convert::toCInt(value);

    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    inst->setY(y);

    return Qnil;
  }

  /*
    Get width
  */
  static VALUE getter_width(VALUE self)
  {
    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    return Convert::toRubyNumber(inst->getWidth());
  }

  /*
    Set width
  */
  static VALUE setter_width(VALUE self, VALUE value)
  {
    int width = Convert::toCInt(value);

    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    inst->setWidth(width);

    return Qnil;
  }

  /*
    Get height
  */
  static VALUE getter_height(VALUE self)
  {
    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    return Convert::toRubyNumber(inst->getHeight());
  }

  /*
    Set height
  */
  static VALUE setter_height(VALUE self, VALUE value)
  {
    int height = Convert::toCInt(value);

    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    inst->setHeight(height);

    return Qnil;
  }

  /*
   Get z
  */
  static VALUE getter_z(VALUE self)
  {
    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    return Convert::toRubyNumber(inst->getZ());
  }

  /*
    Set z
  */
  static VALUE setter_z(VALUE self, VALUE value)
  {
    int z = Convert::toCInt(value);

    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    inst->setZ(z);

    return Qnil;
  }

  /*
   Get ox
  */
  static VALUE getter_ox(VALUE self)
  {
    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    return Convert::toRubyNumber(inst->getter_ox());
  }

  /*
    Set ox
  */
  static VALUE setter_ox(VALUE self, VALUE value)
  {
    int z = Convert::toCInt(value);

    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    inst->setter_ox(z);

    return Qnil;
  }

  /*
   Get oy
  */
  static VALUE getter_oy(VALUE self)
  {
    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    return Convert::toRubyNumber(inst->getter_oy());
  }

  /*
    Set oy
  */
  static VALUE setter_oy(VALUE self, VALUE value)
  {
    int z = Convert::toCInt(value);

    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    inst->setter_oy(z);

    return Qnil;
  }

  /*
   Get opacity
  */
  static VALUE getter_opacity(VALUE self)
  {
    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    return Convert::toRubyNumber(inst->getter_opacity());
  }

  /*
    Set opacity
  */
  static VALUE setter_opacity(VALUE self, VALUE value)
  {
    int z = Convert::toCInt(value);

    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    inst->setter_opacity(z);

    return Qnil;
  }

  /*
   Get back_opacity
  */
  static VALUE getter_back_opacity(VALUE self)
  {
    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    return Convert::toRubyNumber(inst->getter_back_opacity());
  }

  /*
    Set back_opacity
  */
  static VALUE setter_back_opacity(VALUE self, VALUE value)
  {
    int z = Convert::toCInt(value);

    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    inst->setter_back_opacity(z);

    return Qnil;
  }

  /*
   Get contents_opacity
  */
  static VALUE getter_contents_opacity(VALUE self)
  {
    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    return Convert::toRubyNumber(inst->getter_contents_opacity());
  }

  /*
    Set contents_opacity
  */
  static VALUE setter_contents_opacity(VALUE self, VALUE value)
  {
    int z = Convert::toCInt(value);

    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    inst->setter_contents_opacity(z);

    return Qnil;
  }

  /*
    Method dispose
  */

  static VALUE method_dispose(VALUE self)
  {
    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    if (inst == nullptr) {
      return Qnil;
    }

    inst->method_dispose();

    return Qnil;
  }

  static VALUE method_disposed(VALUE self)
  {
    Eng::Window *inst = (Eng::Window *)DATA_PTR(self);
    if (inst == nullptr) {
      return Qnil;
    }

    bool result = inst->method_disposed();

    return result ? Qtrue : Qfalse;
  }
};
}  // namespace It