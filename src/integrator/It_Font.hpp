#pragma once

#include <stdexcept>
#include <string>

#include "AppDefs.h"
#include "Convert.hpp"
#include "engnine/Color.hpp"
#include "engnine/Font.hpp"
#include "integrator/It_Color.hpp"
#include "ruby.h"

// TODO: Implement methods and attributes

namespace It {

using app::String;
using app::StrVectorPtr;
using app::UPtr;
using app::Vector;

class Font {
 public:

  static void integrate()
  {
    VALUE fontClass = rb_define_class("Font", rb_cObject);
    rb_define_alloc_func(fontClass, instance_allocator);

    rb_define_module_function(fontClass, "exist?", RUBY_METHOD_FUNC(method_exist), 1);

    rb_define_module_function(fontClass, "default_name", RUBY_METHOD_FUNC(attrGet_default_name), 0);
    rb_define_module_function(fontClass, "default_name=", RUBY_METHOD_FUNC(attrSet_default_name), 1);

    rb_define_module_function(fontClass, "default_size", RUBY_METHOD_FUNC(attrGet_default_size), 0);
    rb_define_module_function(fontClass, "default_size=", RUBY_METHOD_FUNC(attrSet_default_size), 1);

    rb_define_method(fontClass, "initialize", RUBY_METHOD_FUNC(method_initialize), -1);

    rb_define_method(fontClass, "name", RUBY_METHOD_FUNC(getter_name), 0);
    rb_define_method(fontClass, "name=", RUBY_METHOD_FUNC(setter_name), 1);

    rb_define_method(fontClass, "size", RUBY_METHOD_FUNC(getter_size), 0);
    rb_define_method(fontClass, "size=", RUBY_METHOD_FUNC(setter_size), 1);

    rb_define_method(fontClass, "bold", RUBY_METHOD_FUNC(getter_bold), 0);
    rb_define_method(fontClass, "bold=", RUBY_METHOD_FUNC(setter_bold), 1);

    rb_define_method(fontClass, "italic", RUBY_METHOD_FUNC(getter_italic), 0);
    rb_define_method(fontClass, "italic=", RUBY_METHOD_FUNC(setter_italic), 1);

    rb_define_method(fontClass, "color", RUBY_METHOD_FUNC(getter_color), 0);
    rb_define_method(fontClass, "color=", RUBY_METHOD_FUNC(setter_color), 1);
  }

  // Utils

  static VALUE getRbClass()
  {
    return rb_const_get(rb_cObject, rb_intern("Font"));
  }

  static VALUE createRubyObject(Eng::Font *inst)
  {
    return Data_Wrap_Struct(getRbClass(), instance_mark, instance_free, inst);
  }

  static VALUE getRubyObject(Eng::Font *inst)
  {
    if (inst == nullptr) {
      return Qnil;
    }
    if (inst->rbObj == Qnil) {
      inst->rbObj = createRubyObject(inst);
    }
    return inst->rbObj;
  }

  static Eng::Font *getObjectValue(VALUE rbObj)
  {
    return (Eng::Font *)DATA_PTR(rbObj);
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
    delete static_cast<Eng::Font *>(ptr);
  }

  static void instance_mark(void *ptr)
  {
  }

  /*
    Method initialize
  */

  static VALUE method_initialize(int argc, VALUE *argv, VALUE self)
  {
    Eng::Font *font = nullptr;
    VALUE _names, _size;

    if (argc == 1) {
      rb_scan_args(argc, argv, "1", &_names);
      StrVectorPtr names = Convert::toCStringVector(_names);
      font = new Eng::Font(*names);
    } else if (argc == 2) {
      rb_scan_args(argc, argv, "2", &_names, &_size, _size);
      StrVectorPtr names = Convert::toCStringVector(_names);
      int size = Convert::toCInt(_size);
      font = new Eng::Font(*names, size);
    } else {
      throw std::runtime_error(
        "Font constructor takes 1 or 2 arguments, but received " + std::to_string(argc) + " arguments."
      );
    }

    DATA_PTR(self) = font;
    font->rbObj = self;
    return self;
  }

  static VALUE method_exist(VALUE self)
  {
    return Qnil;
  }

  /*
   Attr default_name
 */

  static VALUE attrGet_default_name(VALUE self)
  {
    // Eng::Rect *inst = (Eng::Rect *)DATA_PTR(self);
    const char *name = "Arial";
    return rb_str_new2(name);
  }

  static VALUE attrSet_default_name(VALUE self, VALUE value)
  {
    // Check_Type(value, T_FIXNUM);
    // unsigned int x = FIX2INT(value);
    // Eng::Rect *inst = (Eng::Rect *)DATA_PTR(self);
    // inst->x = x;
    return value;
  }

  /*
   Attr default_name
 */

  static VALUE attrGet_default_size(VALUE self)
  {
    // Eng::Rect *inst = (Eng::Rect *)DATA_PTR(self);
    const char *name = "Arial";
    return INT2FIX(16);
  }

  static VALUE attrSet_default_size(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    // unsigned int x = FIX2INT(value);
    // Eng::Rect *inst = (Eng::Rect *)DATA_PTR(self);
    // inst->x = x;
    return value;
  }

  /*
    Get name
  */
  static VALUE getter_name(VALUE self)
  {
    Eng::Font *inst = (Eng::Font *)DATA_PTR(self);
    return Convert::toRubyStringArray(
      inst->getter_name()
    );
  }

  /*
    Set name
  */
  static VALUE setter_name(VALUE self, VALUE value)
  {
    StrVectorPtr names = Convert::toCStringVector(value);
    if (names == nullptr) {
      return Qnil;
    }

    Eng::Font *inst = (Eng::Font *)DATA_PTR(self);
    inst->setter_name(*names);

    return value;
  }

  /*
    Get size
  */
  static VALUE getter_size(VALUE self)
  {
    Eng::Font *inst = (Eng::Font *)DATA_PTR(self);
    return Convert::toRubyNumber(inst->getter_size());
  }

  /*
    Set size
  */
  static VALUE setter_size(VALUE self, VALUE value)
  {
    int x = Convert::toCInt(value);

    Eng::Font *inst = (Eng::Font *)DATA_PTR(self);
    inst->setter_size(x);

    return value;
  }

  /*
    Get bold
  */
  static VALUE getter_bold(VALUE self)
  {
    Eng::Font *inst = (Eng::Font *)DATA_PTR(self);
    return Convert::toRubyBool(inst->getter_bold());
  }

  /*
    Set bold
  */
  static VALUE setter_bold(VALUE self, VALUE _value)
  {
    bool value = Convert::toCBool(_value);
    Eng::Font *inst = (Eng::Font *)DATA_PTR(self);
    inst->setter_bold(value);
    return _value;
  }

  /*
    Get italic
  */
  static VALUE getter_italic(VALUE self)
  {
    Eng::Font *inst = (Eng::Font *)DATA_PTR(self);
    return Convert::toRubyBool(inst->getter_italic());
  }

  /*
    Set italic
  */
  static VALUE setter_italic(VALUE self, VALUE _value)
  {
    bool value = Convert::toCBool(_value);
    Eng::Font *inst = (Eng::Font *)DATA_PTR(self);
    inst->setter_italic(value);
    return value;
  }

  /*
    Get color
  */
  static VALUE getter_color(VALUE self)
  {
    Eng::Font *inst = (Eng::Font *)DATA_PTR(self);
    Eng::Color *color = inst->getter_color();
    return Color::getRubyObject(color);
  }

  /*
    Set color
  */
  static VALUE setter_color(VALUE self, VALUE value)
  {
    Eng::Font *inst = (Eng::Font *)DATA_PTR(self);
    inst->setter_color(
      Color::getObjectValue(value)
    );
    return value;
  }
};
}  // namespace It