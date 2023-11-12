#pragma once

#include "engnine/Graphics.hpp"
#include "ruby.h"

namespace It {

class Graphics {
 public:

  static void integrate()
  {
    VALUE graphicsClass = rb_define_class("Graphics", rb_cObject);

    rb_define_module_function(graphicsClass, "update", RUBY_METHOD_FUNC(method_update), 0);
  }

  static VALUE getRbClass()
  {
    return rb_const_get(rb_cObject, rb_intern("Graphics"));
  }

 private:

  static VALUE method_update(VALUE self)
  {
    Eng::Graphics *obj = (Eng::Graphics *)DATA_PTR(self);
    obj->update();
    return Qnil;
  }
};

}  // namespace It