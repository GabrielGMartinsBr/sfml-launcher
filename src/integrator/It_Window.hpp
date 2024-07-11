#pragma once

#include "Convert.hpp"
#include "It_Bitmap.hpp"
#include "base/Log.hpp"
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

    rb_define_method(windowClass, "x", RUBY_METHOD_FUNC(getter_x), 0);
    rb_define_method(windowClass, "x=", RUBY_METHOD_FUNC(setter_x), 1);

    rb_define_method(windowClass, "y", RUBY_METHOD_FUNC(getter_y), 0);
    rb_define_method(windowClass, "y=", RUBY_METHOD_FUNC(setter_y), 1);

    rb_define_method(windowClass, "z", RUBY_METHOD_FUNC(getter_y), 0);
    rb_define_method(windowClass, "z=", RUBY_METHOD_FUNC(setter_y), 1);

    rb_define_method(windowClass, "width", RUBY_METHOD_FUNC(getter_width), 0);
    rb_define_method(windowClass, "width=", RUBY_METHOD_FUNC(setter_width), 1);

    rb_define_method(windowClass, "height", RUBY_METHOD_FUNC(getter_height), 0);
    rb_define_method(windowClass, "height=", RUBY_METHOD_FUNC(setter_height), 1);

    rb_define_method(windowClass, "contents", RUBY_METHOD_FUNC(getter_contents), 0);
    rb_define_method(windowClass, "contents=", RUBY_METHOD_FUNC(setter_contents), 1);
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
      Set Attr windowSkin
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
    inst->setWidth(height);

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
};
}  // namespace It