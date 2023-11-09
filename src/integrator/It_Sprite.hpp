#pragma once

#include "engnine/Bitmap.hpp"
#include "engnine/Sprite.hpp"
#include "ruby.h"

namespace It {

class Sprite {
  // static VALUE spriteClass;

 public:
  static void integrate()
  {
    VALUE spriteClass = rb_define_class("Sprite", rb_cObject);

    rb_define_method(spriteClass, "initialize", RUBY_METHOD_FUNC(method_initialize), 0);

    rb_define_method(spriteClass, "bitmap", RUBY_METHOD_FUNC(attrGet_bitmap), 0);
    rb_define_method(spriteClass, "bitmap=", RUBY_METHOD_FUNC(attrSet_bitmap), 1);
  }

 private:

  static VALUE method_initialize(VALUE self)
  {
    Eng::Sprite *instance = new Eng::Sprite();
    DATA_PTR(self) = instance;
    return self;
  }

  static VALUE attrGet_bitmap(VALUE self)
  {
    Eng::Sprite *inst = (Eng::Sprite *)DATA_PTR(self);
    Eng::Bitmap *bitmap = inst->bitmap;

    if (bitmap == nullptr) {
      return Qnil;
    }

    return inst->bitmap_ptr;
  }

  static VALUE attrSet_bitmap(VALUE self, VALUE value)
  {
    Check_Type(value, T_OBJECT);

    Eng::Sprite *inst = (Eng::Sprite *)DATA_PTR(self);
    Eng::Bitmap *bitmap = (Eng::Bitmap *)DATA_PTR(value);

    inst->bitmap = bitmap;
    inst->bitmap_ptr = value;

    return value;
  }
};

}  // namespace It