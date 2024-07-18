#pragma once

#include "engnine/FileTest.hpp"
#include "integrator/Convert.hpp"
#include "ruby.h"

namespace It {

class FileTest {
 public:

  static void integrate()
  {
    VALUE fileTestModule = rb_define_module("FileTest");

    rb_define_singleton_method(fileTestModule, "exist?", RUBY_METHOD_FUNC(exist), 1);
    rb_define_singleton_method(fileTestModule, "directory?", RUBY_METHOD_FUNC(directory), 1);
    rb_define_singleton_method(fileTestModule, "file?", RUBY_METHOD_FUNC(file), 1);
    rb_define_singleton_method(fileTestModule, "size", RUBY_METHOD_FUNC(size), 1);
    rb_define_singleton_method(fileTestModule, "size2", RUBY_METHOD_FUNC(size), 1);
  }

 private:

  static VALUE exist(VALUE self, VALUE _path)
  {
    app::CStr path = Convert::toCStr(_path);
    bool result = Eng::FileTest::exist(path);
    return result ? Qtrue : Qfalse;
  }

  static VALUE directory(VALUE self, VALUE _path)
  {
    app::CStr path = Convert::toCStr(_path);
    bool result = Eng::FileTest::directory(path);
    return result ? Qtrue : Qfalse;
  }

  static VALUE file(VALUE self, VALUE _path)
  {
    app::CStr path = Convert::toCStr(_path);
    bool result = Eng::FileTest::file(path);
    return result ? Qtrue : Qfalse;
  }

  static VALUE size(VALUE self, VALUE _path)
  {
    app::CStr path = Convert::toCStr(_path);
    return Convert::toRubyNumber(Eng::FileTest::size(path));
  }
};

}  // namespace It