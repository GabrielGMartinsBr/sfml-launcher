#pragma once

#include "engnine/Input.hpp"
#include "ruby.h"

namespace It {

class Input {

 public:

  void integrate()
  {
    VALUE inputClass = rb_define_class("Input", rb_cObject);

    rb_define_module_function(inputClass, "update", RUBY_METHOD_FUNC(method_Update), 0);
  }

  static VALUE method_Update(VALUE self)
  {
    Eng::Input *obj = (Eng::Input *)DATA_PTR(self);
    obj->update();
    return Qnil;
  }
};

}  // namespace It