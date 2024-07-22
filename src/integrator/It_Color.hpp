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
    return Data_Wrap_Struct(getRbClass(), 0, free, inst);
  }

  static VALUE getRubyObject(Eng::Color *inst)
  {
    if (inst == nullptr) {
      return Qnil;
    }
    if (inst->ptr == Qnil) {
      inst->ptr = createRubyObject(inst);
    }
    return inst->ptr;
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

  static Eng::Color *getInstance(VALUE self)
  {
    return (Eng::Color *)DATA_PTR(self);
  }

  static VALUE method_load(VALUE self, VALUE marshaled_data)
  {
    Check_Type(marshaled_data, T_STRING);
    const char *data = RSTRING_PTR(marshaled_data);
    int len = RSTRING_LEN(marshaled_data);

    Eng::Color *color = Eng::Color::deserialize(data, len);

    VALUE rubyObj = Data_Wrap_Struct(self, NULL, free, color);
    return rubyObj;
  }

  static VALUE alloc(VALUE self)
  {
    return self;
  }

  // Initialize

  static VALUE method_initialize(int argc, VALUE *argv, VALUE self)
  {
    Eng::Color *instance;
    VALUE rb_r, rb_g, rb_b, rb_a;

    if (argc == 3) {
      rb_scan_args(argc, argv, "3", &rb_r, &rb_g, &rb_b);
      int r = Convert::toCInt(rb_r);
      int g = Convert::toCInt(rb_g);
      int b = Convert::toCInt(rb_b);
      instance = new Eng::Color(r, g, b);
    } else if (argc == 4) {
      rb_scan_args(argc, argv, "4", &rb_r, &rb_g, &rb_b, &rb_a);
      int r = Convert::toCInt(rb_r);
      int g = Convert::toCInt(rb_g);
      int b = Convert::toCInt(rb_b);
      int a = Convert::toCInt(rb_a);
      instance = new Eng::Color(r, g, b, a);
    } else {
      RbUtils::raiseRuntimeException(
        "Color initialize takes 3 or 4 arguments, but " + std::to_string(argc) + " were received."
      );
      return Qnil;
    }

    instance->ptr = self;
    DATA_PTR(self) = instance;
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
    inst->red = Convert::toCDouble(value);
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
    inst->green = Convert::toCDouble(value);
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
    inst->blue = Convert::toCDouble(value);
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
    inst->alpha = Convert::toCDouble(value);
    return value;
  }

  /*
      Methods
  */

  // Method set

  static VALUE method_set(int argc, VALUE *argv, VALUE self)
  {
    Eng::Color *inst = getInstance(self);
    VALUE _r, _g, _b, _a;

    if (argc == 3) {
      rb_scan_args(argc, argv, "3", &_r, &_g, &_b);
      float r = Convert::toCDouble(_r);
      float g = Convert::toCDouble(_g);
      float b = Convert::toCDouble(_b);
      inst->set(r, g, b);
      return Qnil;
    }

    if (argc == 4) {
      rb_scan_args(argc, argv, "3", &_r, &_g, &_b, &_a);
      float r = Convert::toCDouble(_r);
      float g = Convert::toCDouble(_g);
      float b = Convert::toCDouble(_b);
      float a = Convert::toCDouble(_a);
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
    Eng::Color &i = *getInstance(self);
    char buffer[64];
    sprintf(buffer, "(%f, %f, %f, %f)", i.red, i.green, i.blue, i.alpha);
    return Convert::toRubyString(buffer);
  }
};

}  // namespace It