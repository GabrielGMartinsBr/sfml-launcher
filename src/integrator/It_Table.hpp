#pragma once

#include "Convert.hpp"
#include "engnine/Table.hpp"
#include "ruby.h"

// TODO: Implement methods and attributes

namespace It {

class Table {
 public:

  static void integrate()
  {
    VALUE tableClass = rb_define_class("Table", rb_cObject);

    rb_define_module_function(tableClass, "_load", RUBY_METHOD_FUNC(method_load), 1);

    rb_define_method(tableClass, "initialize", RUBY_METHOD_FUNC(method_initialize), -1);
    rb_define_method(tableClass, "[]", RUBY_METHOD_FUNC(getValue), -1);
    rb_define_method(tableClass, "[]=", RUBY_METHOD_FUNC(setValue), -1);
    rb_define_method(tableClass, "resize", RUBY_METHOD_FUNC(method_resize), -1);
    rb_define_method(tableClass, "xsize", RUBY_METHOD_FUNC(method_xsize), 0);
    rb_define_method(tableClass, "ysize", RUBY_METHOD_FUNC(method_ysize), 0);
    rb_define_method(tableClass, "zsize", RUBY_METHOD_FUNC(method_zsize), 0);
  }

 private:

  static VALUE method_load(VALUE self, VALUE marshaled_data)
  {
    Check_Type(marshaled_data, T_STRING);
    const char *data = RSTRING_PTR(marshaled_data);
    int len = RSTRING_LEN(marshaled_data);

    Eng::Table *table = Eng::Table::deserialize(data, len);

    VALUE table_obj = Data_Wrap_Struct(self, NULL, free, table);
    return table_obj;
  }

  static VALUE method_initialize(int argc, VALUE *argv, VALUE self)
  {
    Eng::Table *instance = nullptr;
    VALUE rb_x, rb_y, rb_z;

    if (argc == 1) {
      rb_scan_args(argc, argv, "1", &rb_x);
      int x = Convert::toCInt(rb_x);
      instance = new Eng::Table(x);
    } else if (argc == 2) {
      rb_scan_args(argc, argv, "2", &rb_x, &rb_y);
      int x = Convert::toCInt(rb_x);
      int y = Convert::toCInt(rb_y);
      instance = new Eng::Table(x, y);
    } else if (argc == 3) {
      rb_scan_args(argc, argv, "3", &rb_x, &rb_y, &rb_z);
      int x = Convert::toCInt(rb_x);
      int y = Convert::toCInt(rb_y);
      int z = Convert::toCInt(rb_z);
      instance = new Eng::Table(x, y, z);
    } else {
      throw std::runtime_error(
        "Table initialize takes exactly 1, 2, or 3 arguments, but received "
        + std::to_string(argc) + " arguments."
      );
    }

    DATA_PTR(self) = instance;
    return self;
  }

  static VALUE getValue(int argc, VALUE *argv, VALUE self)
  {
    Eng::Table *inst = (Eng::Table *)DATA_PTR(self);
    VALUE rb_x, rb_y, rb_z;

    if (argc == 1) {
      rb_scan_args(argc, argv, "1", &rb_x);
      int x = Convert::toCInt(rb_x);
      short value = inst->getValue(x);
      return Convert::toRubyNumber(value);
    }

    if (argc == 2) {
      rb_scan_args(argc, argv, "2", &rb_x, &rb_y);
      int x = Convert::toCInt(rb_x);
      int y = Convert::toCInt(rb_y);
      short value = inst->getValue(x, y);
      return Convert::toRubyNumber(value);
    }

    if (argc == 3) {
      rb_scan_args(argc, argv, "3", &rb_x, &rb_y, &rb_z);
      int x = Convert::toCInt(rb_x);
      int y = Convert::toCInt(rb_y);
      int z = Convert::toCInt(rb_z);
      short value = inst->getValue(x, y, z);
      return Convert::toRubyNumber(value);
    }

    throw std::runtime_error(
      "Method get value takes exactly 1, 2, or 3 arguments, but received "
      + std::to_string(argc) + " arguments."
    );
  }

  static VALUE setValue(int argc, VALUE *argv, VALUE self)
  {
    Eng::Table *inst = (Eng::Table *)DATA_PTR(self);
    VALUE rb_v, rb_x, rb_y, rb_z;

    if (argc == 2) {
      rb_scan_args(argc, argv, "2", &rb_x, &rb_v);
      int x = Convert::toCInt(rb_x);
      int v = Convert::toCInt(rb_v);
      inst->setValue(v, x);
      return Qnil;
    }

    if (argc == 3) {
      rb_scan_args(argc, argv, "3", &rb_x, &rb_y, &rb_v);
      int x = Convert::toCInt(rb_x);
      int y = Convert::toCInt(rb_y);
      int v = Convert::toCInt(rb_v);
      inst->setValue(v, x, y);
      return Qnil;
    }

    if (argc == 4) {
      rb_scan_args(argc, argv, "4", &rb_x, &rb_y, &rb_z, &rb_v);
      int x = Convert::toCInt(rb_x);
      int y = Convert::toCInt(rb_y);
      int z = Convert::toCInt(rb_z);
      int v = Convert::toCInt(rb_v);
      inst->setValue(v, x, y, z);
      return Qnil;
    }

    throw std::runtime_error(
      "Method set value takes exactly 2, 3, or 4 arguments, but received "
      + std::to_string(argc) + " arguments."
    );
  }

  static VALUE method_resize(int argc, VALUE *argv, VALUE self)
  {
    if (argc == 0 || argc > 3) {
      throw std::runtime_error(
        "Method resize takes exactly 1, 2, or 3 arguments, but received "
        + std::to_string(argc) + " arguments."
      );
    }

    return Qnil;
  }

  static VALUE method_xsize(VALUE self)
  {
    Eng::Table *inst = (Eng::Table *)DATA_PTR(self);
    return Convert::toRubyNumber(inst->getXSize());
  }

  static VALUE method_ysize(VALUE self)
  {
    Eng::Table *inst = (Eng::Table *)DATA_PTR(self);
    return Convert::toRubyNumber(inst->getYSize());
  }

  static VALUE method_zsize(VALUE self)
  {
    Eng::Table *inst = (Eng::Table *)DATA_PTR(self);
    return Convert::toRubyNumber(inst->getZSize());
  }
};
}  // namespace It