#pragma once

#include "engnine/Bitmap.hpp"
#include "engnine/Engine.hpp"
#include "engnine/Sprite.hpp"
#include "engnine/Viewport.hpp"
#include "integrator/It_Viewport.hpp"
#include "ruby.h"

namespace It {

class Sprite {
  // static VALUE spriteClass;

 public:
  static void integrate()
  {
    VALUE spriteClass = rb_define_class("Sprite", rb_cObject);

    rb_define_method(spriteClass, "initialize", RUBY_METHOD_FUNC(method_initialize), -1);

    rb_define_method(spriteClass, "x", RUBY_METHOD_FUNC(attrGet_x), 0);
    rb_define_method(spriteClass, "x=", RUBY_METHOD_FUNC(attrSet_x), 1);

    rb_define_method(spriteClass, "y", RUBY_METHOD_FUNC(attrGet_y), 0);
    rb_define_method(spriteClass, "y=", RUBY_METHOD_FUNC(attrSet_y), 1);

    rb_define_method(spriteClass, "z", RUBY_METHOD_FUNC(attrGet_z), 0);
    rb_define_method(spriteClass, "z=", RUBY_METHOD_FUNC(attrSet_z), 1);

    rb_define_method(spriteClass, "ox", RUBY_METHOD_FUNC(attrGet_ox), 0);
    rb_define_method(spriteClass, "ox=", RUBY_METHOD_FUNC(attrSet_ox), 1);

    rb_define_method(spriteClass, "oy", RUBY_METHOD_FUNC(attrGet_oy), 0);
    rb_define_method(spriteClass, "oy=", RUBY_METHOD_FUNC(attrSet_oy), 1);

    rb_define_method(spriteClass, "bitmap", RUBY_METHOD_FUNC(attrGet_bitmap), 0);
    rb_define_method(spriteClass, "bitmap=", RUBY_METHOD_FUNC(attrSet_bitmap), 1);

    rb_define_method(spriteClass, "src_rect", RUBY_METHOD_FUNC(attrGet_src_rect), 0);
    rb_define_method(spriteClass, "src_rect=", RUBY_METHOD_FUNC(attrSet_src_rect), 1);

    rb_define_method(spriteClass, "viewport", RUBY_METHOD_FUNC(attrGet_viewport), 0);

    rb_define_method(spriteClass, "dispose", RUBY_METHOD_FUNC(method_dispose), 0);
    rb_define_method(spriteClass, "disposed", RUBY_METHOD_FUNC(method_disposed), 0);
  }

 private:

  static Eng::Sprite *getInstance(VALUE self)
  {
    return (Eng::Sprite *)DATA_PTR(self);
  }

  /*
    Method initialize
  */

  static VALUE method_initialize(int argc, VALUE *argv, VALUE self)
  {
    if (argc == 0) {
      return overload_initialize1(self);
    } else if (argc == 1) {
      return overload_initialize2(argc, argv, self);
    }

    throw std::runtime_error("Failed to initialize sprite.");
  }

  static VALUE overload_initialize1(VALUE self)
  {
    Eng::Sprite *instance = new Eng::Sprite();
    DATA_PTR(self) = instance;

    Eng::Engine::getInstance().addSprite(self);

    return self;
  }

  static VALUE overload_initialize2(int argc, VALUE *argv, VALUE self)
  {
    VALUE _viewport;
    rb_scan_args(argc, argv, "1", &_viewport);

    Check_Type(_viewport, T_OBJECT);
    Eng::Viewport *viewport = (Eng::Viewport *)DATA_PTR(_viewport);

    Eng::Sprite *instance = new Eng::Sprite(viewport);
    DATA_PTR(self) = instance;

    Eng::Engine::getInstance().addSprite(self);

    return self;
  }

  /*
    Attr x
  */

  static VALUE attrGet_x(VALUE self)
  {
    Eng::Sprite *inst = (Eng::Sprite *)DATA_PTR(self);
    return INT2FIX(inst->getX());
  }

  static VALUE attrSet_x(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    unsigned int x = FIX2INT(value);
    Eng::Sprite *inst = (Eng::Sprite *)DATA_PTR(self);
    inst->setX(x);
    return value;
  }

  /*
    Attr y
  */

  static VALUE attrGet_y(VALUE self)
  {
    Eng::Sprite *inst = (Eng::Sprite *)DATA_PTR(self);
    return INT2FIX(inst->getY());
  }

  static VALUE attrSet_y(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    unsigned int y = FIX2INT(value);
    Eng::Sprite *inst = (Eng::Sprite *)DATA_PTR(self);
    inst->setY(y);
    return value;
  }

  /*
   Attr z
  */

  static VALUE attrGet_z(VALUE self)
  {
    int z = getInstance(self)->getZ();
    return INT2FIX(z);
  }

  static VALUE attrSet_z(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    int z = FIX2INT(value);
    getInstance(self)->setZ(z);
    return value;
  }

  /*
   Attr ox
  */

  static VALUE attrGet_ox(VALUE self)
  {
    int ox = getInstance(self)->getOx();
    return INT2FIX(ox);
  }

  static VALUE attrSet_ox(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    int ox = FIX2INT(value);
    getInstance(self)->setOx(ox);
    return value;
  }

  /*
   Attr oy
  */

  static VALUE attrGet_oy(VALUE self)
  {
    int oy = getInstance(self)->getOy();
    return INT2FIX(oy);
  }

  static VALUE attrSet_oy(VALUE self, VALUE value)
  {
    Check_Type(value, T_FIXNUM);
    int oy = FIX2INT(value);
    getInstance(self)->setOy(oy);
    return value;
  }

  /*
    Attr bitmap
  */

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

    return value;
  }

  /*
    Attr src_rect
  */

  static VALUE attrGet_src_rect(VALUE self)
  {
    Eng::Sprite *inst = (Eng::Sprite *)DATA_PTR(self);
    Eng::Rect *rect = inst->getSrcRect();

    if (rect == nullptr) {
      return Qnil;
    }

    return inst->bitmap_ptr;
  }

  static VALUE attrSet_src_rect(VALUE self, VALUE value)
  {
    Check_Type(value, T_OBJECT);

    Eng::Sprite *inst = (Eng::Sprite *)DATA_PTR(self);
    Eng::Rect *rect = (Eng::Rect *)DATA_PTR(value);

    inst->setSrcRect(rect);
    inst->bitmap_ptr = value;

    return value;
  }

  /*
    Method viewport
  */

  static VALUE attrGet_viewport(VALUE self)
  {
    Eng::Sprite *inst = (Eng::Sprite *)DATA_PTR(self);
    Eng::Viewport *viewport = inst->getViewport();

    if (viewport == nullptr) {
      return Qnil;
    }

    VALUE viewportClass = Viewport::getRbClass();
    return Data_Wrap_Struct(viewportClass, 0, free, viewport);
  }

  /*
    Method dispose
  */

  static VALUE method_dispose(VALUE self)
  {
    Eng::Sprite *inst = (Eng::Sprite *)DATA_PTR(self);
    inst->dispose();
    return Qnil;
  }

  /*
    Method disposed
  */

  static VALUE method_disposed(VALUE self)
  {
    Eng::Sprite *inst = (Eng::Sprite *)DATA_PTR(self);
    bool isDisposed = inst->disposed();
    return isDisposed ? Qtrue : Qfalse;
  }
};

}  // namespace It