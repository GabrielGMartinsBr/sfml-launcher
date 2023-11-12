#pragma once

#include "GlobalTest.h"
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

    rb_define_method(spriteClass, "dispose", RUBY_METHOD_FUNC(method_dispose), 0);
    rb_define_method(spriteClass, "disposed", RUBY_METHOD_FUNC(method_disposed), 0);
  }

 private:

  static VALUE method_initialize(VALUE self)
  {
    Log::out() << "addSprite";
    Eng::Sprite *instance = new Eng::Sprite();
    globalInst.addSprite(self);
    DATA_PTR(self) = instance;
    return self;
  }

  static VALUE attrGet_bitmap(VALUE self)
  {
    Eng::Sprite *inst = (Eng::Sprite *)DATA_PTR(self);
    Eng::Bitmap *bitmap = inst->getBitmap();

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

    inst->setBitmap(bitmap);
    inst->bitmap_ptr = value;

    // inst->atualizar();

    return value;
  }

  static VALUE method_dispose(VALUE self)
  {
    Eng::Sprite *inst = (Eng::Sprite *)DATA_PTR(self);
    inst->dispose();
    return Qnil;
  }

  static VALUE method_disposed(VALUE self)
  {
    Eng::Sprite *inst = (Eng::Sprite *)DATA_PTR(self);
    bool isDisposed = inst->disposed();
    return isDisposed ? Qtrue : Qfalse;
  }
};

}  // namespace It