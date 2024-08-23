#pragma once

#include "Convert.hpp"
#include "RbUtils.hpp"
#include "engnine/Rect.hpp"
#include "ruby.h"

namespace It {

class Rect {
 public:
  static void integrate()
  {
    VALUE rectClass = rb_define_class("Rect", rb_cObject);
    rb_define_alloc_func(rectClass, instance_allocator);

    // Serialize

    rb_define_module_function(rectClass, "_load", RUBY_METHOD_FUNC(method_load), 1);
    rb_define_module_function(rectClass, "_dump", RUBY_METHOD_FUNC(method_dump), 1);

    rb_define_method(rectClass, "=", RUBY_METHOD_FUNC(operator_bind), 1);
    rb_define_method(rectClass, "==", RUBY_METHOD_FUNC(operator_equal), 1);

    // Initialize
    rb_define_method(rectClass, "initialize", RUBY_METHOD_FUNC(initialize), 4);

    // Props

    rb_define_method(rectClass, "x", RUBY_METHOD_FUNC(getter_x), 0);
    rb_define_method(rectClass, "x=", RUBY_METHOD_FUNC(setter_x), 1);

    rb_define_method(rectClass, "y", RUBY_METHOD_FUNC(getter_y), 0);
    rb_define_method(rectClass, "y=", RUBY_METHOD_FUNC(setter_y), 1);

    rb_define_method(rectClass, "width", RUBY_METHOD_FUNC(getter_width), 0);
    rb_define_method(rectClass, "width=", RUBY_METHOD_FUNC(setter_width), 1);

    rb_define_method(rectClass, "height", RUBY_METHOD_FUNC(getter_height), 0);
    rb_define_method(rectClass, "height=", RUBY_METHOD_FUNC(setter_height), 1);

    // Methods

    rb_define_method(rectClass, "set", RUBY_METHOD_FUNC(method_set), 4);
    rb_define_method(rectClass, "empty", RUBY_METHOD_FUNC(method_empty), 0);

    rb_define_method(rectClass, "to_s", RUBY_METHOD_FUNC(method_to_s), 0);
  }

  // Utils

  static VALUE getRbClass()
  {
    return rb_const_get(rb_cObject, rb_intern("Rect"));
  }

  static VALUE createRubyObject(Eng::Rect *rect)
  {
    return Data_Wrap_Struct(getRbClass(), instance_mark, instance_free, rect);
  }

  static VALUE getRubyObject(Eng::Rect *rect)
  {
    if (rect == nullptr) {
      return Qnil;
    }
    if (rect->rbObj == Qnil) {
      rect->rbObj = createRubyObject(rect);
    }
    return rect->rbObj;
  }

  static Eng::Rect *getObjectValue(VALUE self)
  {
    return (Eng::Rect *)DATA_PTR(self);
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
    delete static_cast<Eng::Rect *>(ptr);
  }

  static void instance_mark(void *ptr)
  {
  }

  /*
    Deserialize / Marshal load
  */

  static VALUE method_load(VALUE self, VALUE marshaled_data)
  {
    Check_Type(marshaled_data, T_STRING);
    const char *data = RSTRING_PTR(marshaled_data);
    int len = RSTRING_LEN(marshaled_data);

    Eng::Rect *rect = Eng::Rect::deserialize(data, len);
    VALUE value = getRubyObject(rect);
    return value;
  }

  /*
    Serialize / Marshal dump
  */

  static VALUE method_dump(VALUE self, VALUE arg)
  {
    char *data = new char[Eng::Rect::SERIAL_SIZE];

    Eng::Rect *inst = getObjectValue(self);
    inst->serialize(data);

    VALUE str = rb_str_new(data, Eng::Rect::SERIAL_SIZE);
    delete[] data;

    return str;
  }

  /*
    Operators
  */

  static VALUE operator_bind(VALUE self, VALUE value)
  {
    if (rb_class_of(self) != rb_class_of(value)) {
      RbUtils::raiseCantConvertError(
        rb_class_of(self),
        rb_class_of(value)
      );
      return Qnil;
    }

    Eng::Rect *inst = getObjectValue(self);
    Eng::Rect *other = getObjectValue(value);

    *inst = *other;
    return self;
  }

  static VALUE operator_equal(VALUE self, VALUE value)
  {
    if (rb_class_of(self) != rb_class_of(value)) {
      return Qfalse;
    }

    Eng::Rect *inst = getObjectValue(self);
    Eng::Rect *other = getObjectValue(value);

    bool isEqual = *inst == *other;
    return Convert::toRubyBool(isEqual);
  }

  /*
    Method initialize
  */

  static VALUE initialize(VALUE self, VALUE _x, VALUE _y, VALUE _w, VALUE _h)
  {
    unsigned int x = Convert::toCInt(_x);
    unsigned int y = Convert::toCInt(_y);
    unsigned int width = Convert::toCInt(_w);
    unsigned int height = Convert::toCInt(_h);

    Eng::Rect *inst = new Eng::Rect(self, x, y, width, height);
    DATA_PTR(self) = inst;

    return self;
  }

  /*
    Attr x
  */

  static VALUE getter_x(VALUE self)
  {
    Eng::Rect *inst = Rect::getObjectValue(self);
    return inst->x.getVALUE();
  }

  static VALUE setter_x(VALUE self, VALUE value)
  {
    Eng::Rect *inst = Rect::getObjectValue(self);
    return inst->x.setVALUE(value);
  }

  /*
    Attr y
  */

  static VALUE getter_y(VALUE self)
  {
    Eng::Rect *inst = Rect::getObjectValue(self);
    return inst->y.getVALUE();
  }

  static VALUE setter_y(VALUE self, VALUE value)
  {
    Eng::Rect *inst = Rect::getObjectValue(self);
    return inst->y.setVALUE(value);
  }

  /*
    Attr width
  */

  static VALUE getter_width(VALUE self)
  {
    Eng::Rect *inst = Rect::getObjectValue(self);
    return inst->width.getVALUE();
  }

  static VALUE setter_width(VALUE self, VALUE value)
  {
    Eng::Rect *inst = Rect::getObjectValue(self);
    return inst->width.setVALUE(value);
  }

  /*
    Attr height
  */

  static VALUE getter_height(VALUE self)
  {
    Eng::Rect *inst = Rect::getObjectValue(self);
    return inst->height.getVALUE();
  }

  static VALUE setter_height(VALUE self, VALUE value)
  {
    Eng::Rect *inst = Rect::getObjectValue(self);
    return inst->height.setVALUE(value);
  }

  // Method set
  static VALUE method_set(VALUE self, VALUE xVal, VALUE yVal, VALUE widthVal, VALUE heightVal)
  {
    Eng::Rect *inst = Rect::getObjectValue(self);
    inst->setVALUES(xVal, yVal, widthVal, heightVal);
    return Qnil;
  }

  // Method empty

  static VALUE method_empty(VALUE self)
  {
    auto inst = Rect::getObjectValue(self);
    inst->method_empty();
    return self;
  }

  // Method to_s

  static VALUE method_to_s(VALUE self)
  {
    Eng::Rect *inst = getObjectValue(self);
    char buffer[64];
    sprintf(buffer, "(%i, %i, %i, %i)", inst->x.get(), inst->y.get(), inst->width.get(), inst->height.get());
    return Convert::toRubyString(buffer);
  }
};

}  // namespace It