#pragma once

#include "engnine/Sprite.hpp"
#include "ruby.h"

namespace It {

class Sprite {
 public:
  static void integrate()
  {
    VALUE rbc_Color = rb_define_class("Sprite", rb_cObject);

    rb_define_method(rbc_Color, "initialize", RUBY_METHOD_FUNC(method_initialize), 0);
  }

 private:

  static VALUE method_initialize(VALUE self)
  {
    Eng::Sprite *instance = new Eng::Sprite();
    DATA_PTR(self) = instance;
    return self;
  }
};

}  // namespace It