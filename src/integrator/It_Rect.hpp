#pragma once

#include "Convert.hpp"
#include "engnine/Rect.hpp"
#include "ruby.h"

namespace It {

class Rect {
 public:
  static void integrate()
  {
    VALUE rectClass = rb_define_class("Rect", rb_cObject);

    rb_define_method(rectClass, "initialize", RUBY_METHOD_FUNC(initialize), 4);

    rb_define_method(rectClass, "x", RUBY_METHOD_FUNC(getter_x), 0);
    rb_define_method(rectClass, "x=", RUBY_METHOD_FUNC(setter_x), 1);

    rb_define_method(rectClass, "y", RUBY_METHOD_FUNC(getter_y), 0);
    rb_define_method(rectClass, "y=", RUBY_METHOD_FUNC(setter_y), 1);

    rb_define_method(rectClass, "width", RUBY_METHOD_FUNC(getter_width), 0);
    rb_define_method(rectClass, "width=", RUBY_METHOD_FUNC(setter_width), 1);

    rb_define_method(rectClass, "height", RUBY_METHOD_FUNC(getter_height), 0);
    rb_define_method(rectClass, "height=", RUBY_METHOD_FUNC(setter_height), 1);

    rb_define_method(rectClass, "set", RUBY_METHOD_FUNC(method_set), 4);
  }

  static VALUE getRbClass()
  {
    return rb_const_get(rb_cObject, rb_intern("Rect"));
  }

  static VALUE createRubyObject(Eng::Rect *rect)
  {
    VALUE rectClass = Rect::getRbClass();
    return Data_Wrap_Struct(rectClass, 0, free, rect);
  }

  static VALUE getRubyObject(Eng::Rect *rect)
  {
    if (rect == nullptr) {
      return Qnil;
    }
    if (rect->ptr == Qnil) {
      rect->ptr = It::Rect::createRubyObject(rect);
    }
    return rect->ptr;
  }

  static Eng::Rect *getObjectValue(VALUE self)
  {
    return (Eng::Rect *)DATA_PTR(self);
  }

 private:

  static VALUE initialize(VALUE self, VALUE _x, VALUE _y, VALUE _w, VALUE _h)
  {
    unsigned int x = Convert::toCInt(_x);
    unsigned int y = Convert::toCInt(_y);
    unsigned int width = Convert::toCInt(_w);
    unsigned int height = Convert::toCInt(_h);

    Eng::Rect *instance = new Eng::Rect(x, y, width, height);
    DATA_PTR(self) = instance;

    return self;
  }

  /*
    Attr x
  */

  static VALUE getter_x(VALUE self)
  {
    auto inst = It::Rect::getObjectValue(self);
    return Convert::toRubyNumber(inst->x);
  }

  static VALUE setter_x(VALUE self, VALUE value)
  {
    unsigned int x = Convert::toCInt(value);
    auto inst = It::Rect::getObjectValue(self);
    inst->x = x;
    return value;
  }

  /*
    Attr y
  */

  static VALUE getter_y(VALUE self)
  {
    auto inst = It::Rect::getObjectValue(self);
    return Convert::toRubyNumber(inst->y);
  }

  static VALUE setter_y(VALUE self, VALUE value)
  {
    unsigned int y = Convert::toCInt(value);
    auto inst = It::Rect::getObjectValue(self);
    inst->y = y;
    return value;
  }

  /*
    Attr width
  */

  static VALUE getter_width(VALUE self)
  {
    auto inst = It::Rect::getObjectValue(self);
    return Convert::toRubyNumber(inst->width);
  }

  static VALUE setter_width(VALUE self, VALUE value)
  {
    unsigned int width = Convert::toCInt(value);
    auto inst = It::Rect::getObjectValue(self);
    inst->width = width;
    return value;
  }

  /*
    Attr height
  */

  static VALUE getter_height(VALUE self)
  {
    auto inst = It::Rect::getObjectValue(self);
    return Convert::toRubyNumber(inst->height);
  }

  static VALUE setter_height(VALUE self, VALUE value)
  {
    unsigned int height = Convert::toCInt(value);
    auto inst = It::Rect::getObjectValue(self);
    inst->height = height;
    return value;
  }

  // Method set
  static VALUE method_set(VALUE self, VALUE _x, VALUE _y, VALUE _width, VALUE _height)
  {
    int x = Convert::toCInt(_x);
    int y = Convert::toCInt(_y);
    int width = Convert::toCInt(_width);
    int height = Convert::toCInt(_height);

    auto inst = It::Rect::getObjectValue(self);
    inst->set(x, y, width, height);

    return Qnil;
  }
};

}  // namespace It