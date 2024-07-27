#pragma once

#include "engnine/Autotiles.h"
#include "engnine/Bitmap.h"
#include "integrator/Convert.hpp"
#include "integrator/It_Bitmap.hpp"
#include "ruby.h"

namespace It {

class Autotiles {

 public:

  static void integrate()
  {
    VALUE autotilesRb = rb_define_class("TilemapAutotiles", rb_cObject);
    rb_define_alloc_func(autotilesRb, instance_allocator);

    // rb_define_method(autotilesRb, "initialize", RUBY_METHOD_FUNC(initialize), 0);

    rb_define_method(autotilesRb, "[]", RUBY_METHOD_FUNC(getter), 1);
    rb_define_method(autotilesRb, "[]=", RUBY_METHOD_FUNC(setter), 2);
  }

  // Utils

  static VALUE getRbClass()
  {
    return rb_const_get(rb_cObject, rb_intern("TilemapAutotiles"));
  }

  static VALUE createRubyObject(Eng::Autotiles *inst)
  {
    return Data_Wrap_Struct(getRbClass(), instance_mark, instance_free, inst);
  }

  static VALUE getRubyObject(Eng::Autotiles *inst)
  {
    if (inst == nullptr) {
      return Qnil;
    }
    if (inst->rbObj == Qnil) {
      inst->rbObj = createRubyObject(inst);
    }
    return inst->rbObj;
  }

  static Eng::Autotiles *getObjectValue(VALUE rbObj)
  {
    return (Eng::Autotiles *)DATA_PTR(rbObj);
  }

 private:

  /*
    Allocator
  */

  static VALUE instance_allocator(VALUE instanceClass)
  {
    return Data_Wrap_Struct(instanceClass, instance_mark, instance_free, nullptr);
  }

  /*
    Deallocator
  */

  static void instance_free(void *ptr)
  {
    delete static_cast<Eng::Autotiles *>(ptr);
  }

  static void instance_mark(void *ptr)
  {
    Eng::Autotiles *inst = static_cast<Eng::Autotiles *>(ptr);
    Eng::Bitmap *bp;
    for (int i = 0; i < 7; i++) {
      bp = inst->getter(i);
      if (bp == nullptr || bp->rbObj == Qnil) {
        continue;
      }
      rb_gc_mark(bp->rbObj);
    }
  }

  /*
    Method initialize
  */

  // static VALUE initialize(int argc, VALUE *argv, VALUE self)
  // {
  //   Eng::Autotiles *inst;
  //   DATA_PTR(self) = inst;
  //   inst->ptr = self;
  //   return self;
  // }

  /*
      Getter
  */
  static VALUE getter(VALUE self, VALUE _index)
  {
    int index = Convert::toCInt(_index);
    if (index < 0 || index > 6) {
      RbUtils::raiseRuntimeException(
        "Autotiles has an index range from 0 to 6, but " + std::to_string(index) + " were received."
      );
      return Qnil;
    }
    Eng::Autotiles *inst = getObjectValue(self);

    Eng::Bitmap *value = inst->getter(index);
    return It::Bitmap::getRubyObject(value);
  }

  /*
      Setter
  */
  static VALUE setter(VALUE self, VALUE _index, VALUE _value)
  {
    int index = Convert::toCInt(_index);
    if (index < 0 || index > 6) {
      RbUtils::raiseRuntimeException(
        "Autotiles has an index range from 0 to 6, but " + std::to_string(index) + " were received."
      );
      return Qnil;
    }
    Eng::Autotiles *inst = It::Autotiles::getObjectValue(self);
    Eng::Bitmap *value = It::Bitmap::getObjectValue(_value);

    // Log::out() << " -> value index: " << index << " ptr: " << value->rbObj;

    inst->setter(index, value);
    return _value;
  }
};

}  // namespace It