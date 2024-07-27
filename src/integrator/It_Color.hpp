#pragma once

#include "Convert.hpp"
#include "RbUtils.hpp"
#include "engnine/Color.hpp"
#include "ruby.h"

namespace It {

class Color {
 public:

  static void integrate()
  {
    VALUE colorClass = rb_define_class("Color", rb_cObject);
    rb_define_alloc_func(colorClass, instance_allocator);

    // Serialize

    rb_define_module_function(colorClass, "_load", RUBY_METHOD_FUNC(method_load), 1);

    // Initialize

    rb_define_method(colorClass, "initialize", RUBY_METHOD_FUNC(method_initialize), -1);

    // Props

    rb_define_method(colorClass, "red", RUBY_METHOD_FUNC(setter_red), 0);
    rb_define_method(colorClass, "red=", RUBY_METHOD_FUNC(getter_red), 1);

    rb_define_method(colorClass, "green", RUBY_METHOD_FUNC(setter_green), 0);
    rb_define_method(colorClass, "green=", RUBY_METHOD_FUNC(getter_green), 1);

    rb_define_method(colorClass, "blue", RUBY_METHOD_FUNC(setter_blue), 0);
    rb_define_method(colorClass, "blue=", RUBY_METHOD_FUNC(getter_blue), 1);

    rb_define_method(colorClass, "alpha", RUBY_METHOD_FUNC(setter_alpha), 0);
    rb_define_method(colorClass, "alpha=", RUBY_METHOD_FUNC(getter_alpha), 1);

    // Methods

    rb_define_method(colorClass, "set", RUBY_METHOD_FUNC(method_set), -1);

    rb_define_method(colorClass, "to_s", RUBY_METHOD_FUNC(method_to_s), 0);
  }

  // Utils

  static VALUE getRbClass()
  {
    return rb_const_get(rb_cObject, rb_intern("Color"));
  }

  static VALUE createRubyObject(Eng::Color *inst)
  {
    return Data_Wrap_Struct(getRbClass(), instance_mark, instance_free, inst);
  }

  static VALUE getRubyObject(Eng::Color *inst)
  {
    if (inst == nullptr) {
      return Qnil;
    }
    if (inst->rbObj == Qnil) {
      inst->rbObj = createRubyObject(inst);
    }
    return inst->rbObj;
  }

  static Eng::Color *getObjectValue(VALUE rbObj)
  {
    return (Eng::Color *)DATA_PTR(rbObj);
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
    if (ptr == nullptr) return;
    // Log::out() << "[[Color_free]]";
    delete static_cast<Eng::Color *>(ptr);
  }

  static void instance_mark(void *ptr)
  {
  }

  static VALUE method_load(VALUE self, VALUE marshaled_data)
  {
    Check_Type(marshaled_data, T_STRING);
    const char *data = RSTRING_PTR(marshaled_data);
    int len = RSTRING_LEN(marshaled_data);

    Eng::Color *color = Eng::Color::deserialize(data, len);
    VALUE value = getRubyObject(color);
    return value;
  }

  // Initialize

  static VALUE method_initialize(int argc, VALUE *argv, VALUE self)
  {
    Eng::Color *instance;
    VALUE rb_r, rb_g, rb_b, rb_a;

    if (argc == 3) {
      rb_scan_args(argc, argv, "3", &rb_r, &rb_g, &rb_b);
      double r = Convert::toCDouble2(rb_r);
      double g = Convert::toCDouble2(rb_g);
      double b = Convert::toCDouble2(rb_b);
      instance = new Eng::Color(r, g, b);
    } else if (argc == 4) {
      rb_scan_args(argc, argv, "4", &rb_r, &rb_g, &rb_b, &rb_a);
      double r = Convert::toCDouble2(rb_r);
      double g = Convert::toCDouble2(rb_g);
      double b = Convert::toCDouble2(rb_b);
      double a = Convert::toCDouble2(rb_a);
      instance = new Eng::Color(r, g, b, a);
    } else {
      RbUtils::raiseRuntimeException(
        "Color initialize takes 3 or 4 arguments, but " + std::to_string(argc) + " were received."
      );
      return Qnil;
    }

    DATA_PTR(self) = instance;
    instance->rbObj = self;
    return self;
  }

  /*
      Props
  */

  // Getter red

  static VALUE getter_red(VALUE self)
  {
    Eng::Color *inst = getObjectValue(self);
    return Convert::toRubyDouble(
      inst->red
    );
  }

  // Setter red

  static VALUE setter_red(VALUE self, VALUE value)
  {
    Eng::Color *inst = getObjectValue(self);
    inst->red = Convert::toCDouble2(value);
    return value;
  }

  // Getter green

  static VALUE getter_green(VALUE self)
  {
    Eng::Color *inst = getObjectValue(self);
    return Convert::toRubyDouble(
      inst->green
    );
  }

  // Setter green

  static VALUE setter_green(VALUE self, VALUE value)
  {
    Eng::Color *inst = getObjectValue(self);
    inst->green = Convert::toCDouble2(value);
    return value;
  }

  // Getter blue

  static VALUE getter_blue(VALUE self)
  {
    Eng::Color *inst = getObjectValue(self);
    return Convert::toRubyDouble(
      inst->blue
    );
  }

  // Setter blue

  static VALUE setter_blue(VALUE self, VALUE value)
  {
    Eng::Color *inst = getObjectValue(self);
    inst->blue = Convert::toCDouble2(value);
    return value;
  }

  // Getter alpha

  static VALUE getter_alpha(VALUE self)
  {
    Eng::Color *inst = getObjectValue(self);
    return Convert::toRubyDouble(
      inst->alpha
    );
  }

  // Setter alpha

  static VALUE setter_alpha(VALUE self, VALUE value)
  {
    Eng::Color *inst = getObjectValue(self);
    inst->alpha = Convert::toCDouble2(value);
    return value;
  }

  /*
      Methods
  */

  // Method set

  static VALUE method_set(int argc, VALUE *argv, VALUE self)
  {
    Eng::Color *inst = getObjectValue(self);
    VALUE _r, _g, _b, _a;

    if (argc == 3) {
      rb_scan_args(argc, argv, "3", &_r, &_g, &_b);
      float r = Convert::toCDouble2(_r);
      float g = Convert::toCDouble2(_g);
      float b = Convert::toCDouble2(_b);
      inst->set(r, g, b);
      return Qnil;
    }

    if (argc == 4) {
      rb_scan_args(argc, argv, "3", &_r, &_g, &_b, &_a);
      float r = Convert::toCDouble2(_r);
      float g = Convert::toCDouble2(_g);
      float b = Convert::toCDouble2(_b);
      float a = Convert::toCDouble2(_a);
      inst->set(r, g, b, a);
      return Qnil;
    }

    RbUtils::raiseRuntimeException(
      "Color set method takes 3 or 4 arguments, but " + std::to_string(argc) + " were received."
    );
    return Qnil;
  }

  // Method to_s

  static VALUE method_to_s(VALUE self)
  {
    Eng::Color &i = *getObjectValue(self);
    char buffer[64];
    sprintf(buffer, "(%f, %f, %f, %f)", i.red, i.green, i.blue, i.alpha);
    return Convert::toRubyString(buffer);
  }
};

}  // namespace It