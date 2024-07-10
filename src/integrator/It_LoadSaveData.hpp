#pragma once

#include "Convert.hpp"
#include "loaders/RxdataLoader.hpp"
#include "ruby.h"

// TODO: Implement save method

namespace It {

class LoadSaveData {
 public:

  static void integrate()
  {
    rb_define_global_function("load_data", RUBY_METHOD_FUNC(function_loadData), 1);
    rb_define_global_function("save_data", RUBY_METHOD_FUNC(function_saveData), 2);
  }

 private:
  static VALUE function_loadData(VALUE self, VALUE rb_filename)
  {
    app::CStr filename = Convert::toCString(rb_filename);
    VALUE obj = RxdataLoader::load(filename);
    return obj;
  }

  static VALUE function_saveData(VALUE self, VALUE oj, VALUE filename)
  {
    return Qnil;
  }
};
}  // namespace It