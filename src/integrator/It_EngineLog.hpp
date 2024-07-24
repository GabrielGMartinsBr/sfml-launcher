#pragma once

#include <iostream>
#include <ostream>

#include "AppDefs.h"
#include "integrator/Convert.hpp"
#include "ruby.h"

namespace It {

struct EngineLog {
  static void integrate()
  {
    VALUE logModule = rb_define_module("Log");
    VALUE outClass = rb_define_class("LogOut", rb_cObject);
    VALUE outLnClass = rb_define_class("LogOutLn", rb_cObject);
    VALUE errClass = rb_define_class("LogErr", rb_cObject);
    VALUE errLnClass = rb_define_class("LogErrLn", rb_cObject);

    rb_define_const(logModule, "ENDL", Convert::toRubyString("\n"));
    rb_define_singleton_method(logModule, "endl", RUBY_METHOD_FUNC(getter_endl), 0);

    rb_define_module_function(logModule, "out", RUBY_METHOD_FUNC(method_out), -1);
    rb_define_module_function(logModule, "outl", RUBY_METHOD_FUNC(method_outl), -1);

    rb_define_module_function(logModule, "err", RUBY_METHOD_FUNC(method_err), -1);
    rb_define_module_function(logModule, "errl", RUBY_METHOD_FUNC(method_errl), -1);

    rb_define_method(outClass, "<<", RUBY_METHOD_FUNC(method_out_insertion), 1);
    rb_define_method(outLnClass, "<<", RUBY_METHOD_FUNC(method_outl_insertion), 1);
    rb_define_method(errClass, "<<", RUBY_METHOD_FUNC(method_err_insertion), 1);
    rb_define_method(errLnClass, "<<", RUBY_METHOD_FUNC(method_errl_insertion), 1);
  }

 private:

  static VALUE getter_endl(VALUE self)
  {
    VALUE logModule = rb_const_get(rb_cObject, rb_intern("Log"));
    return rb_const_get(logModule, rb_intern("ENDL"));
  }

  // Class creators

  static VALUE method_out(int argc, VALUE *argv, VALUE self)
  {
    if (argc == 1) {
      VALUE value;
      rb_scan_args(argc, argv, "1", &value);
      log(std::cout, value);
      std::cout.flush();
    }

    VALUE logOutClass = rb_const_get(rb_cObject, rb_intern("LogOut"));
    VALUE outRubyObj = Data_Wrap_Struct(logOutClass, 0, nullptr, nullptr);
    return outRubyObj;
  }

  static VALUE method_outl(int argc, VALUE *argv, VALUE self)
  {
    if (argc == 1) {
      VALUE value;
      rb_scan_args(argc, argv, "1", &value);
      log(std::cout, value);
      std::cout << std::endl;
    }

    VALUE logOutClass = rb_const_get(rb_cObject, rb_intern("LogOutLn"));
    VALUE outRubyObj = Data_Wrap_Struct(logOutClass, 0, nullptr, nullptr);
    return outRubyObj;
  }

  static VALUE method_err(int argc, VALUE *argv, VALUE self)
  {
    if (argc == 1) {
      VALUE value;
      rb_scan_args(argc, argv, "1", &value);
      log(std::cerr, value);
      std::cerr.flush();
    }

    VALUE logOutClass = rb_const_get(rb_cObject, rb_intern("LogErr"));
    VALUE outRubyObj = Data_Wrap_Struct(logOutClass, 0, nullptr, nullptr);
    return outRubyObj;
  }

  static VALUE method_errl(int argc, VALUE *argv, VALUE self)
  {
    if (argc == 1) {
      VALUE value;
      rb_scan_args(argc, argv, "1", &value);
      log(std::cerr, value);
      std::cerr << std::endl;
    }

    VALUE logOutClass = rb_const_get(rb_cObject, rb_intern("LogErrLn"));
    VALUE outRubyObj = Data_Wrap_Struct(logOutClass, 0, nullptr, nullptr);
    return outRubyObj;
  }

  // Insertion operators

  static VALUE method_out_insertion(VALUE self, VALUE value)
  {
    log(std::cout, value);
    std::cout.flush();
    return self;
  }

  static VALUE method_outl_insertion(VALUE self, VALUE value)
  {
    log(std::cout, value);
    std::cout << std::endl;
    return self;
  }

  static VALUE method_err_insertion(VALUE self, VALUE value)
  {
    log(std::cerr, value);
    std::cerr.flush();
    return self;
  }

  static VALUE method_errl_insertion(VALUE self, VALUE value)
  {
    log(std::cerr, value);
    std::cerr << std::endl;
    return self;
  }

  // Standard log function

  static void log(std::ostream &output_stream, VALUE _value)
  {
    if (_value == Qtrue || _value == Qfalse) {
      bool value = Convert::toCBool(_value);
      output_stream << (value ? "true" : "false");
      return;
    }

    if (TYPE(_value) == T_FLOAT) {
      double value = Convert::toCDouble2(_value);
      output_stream << value;
      return;
    }

    if (TYPE(_value) == T_FIXNUM) {
      int value = Convert::toCInt(_value);
      output_stream << value;
      return;
    }

    if (TYPE(_value) == T_STRING) {
      app::CStr value = Convert::toCStr(_value);
      output_stream << value;
      return;
    }

    rb_p(_value);
  }
};

}  // namespace It