#pragma once

#include <ruby.h>

#include "AppDefs.h"
#include "aeon/base/AeonIntegratorBase.hpp"
#include "aeon/window/AeonButtonElement.h"

namespace ae {

using app::SPtr;

struct AeButtonIntegrator : public AeonIntegratorBase<AeonButtonElement> {
  static VALUE classObject;

  static void integrate(VALUE aeonModule);

  static VALUE instanceAllocator(VALUE instanceClass);
  static void instanceFree(void *ptr);
  static void instanceMark(void *ptr);

  static VALUE initialize(int argc, VALUE *argv, VALUE self);

  static VALUE setPosition(VALUE self, VALUE x, VALUE y);
  static VALUE setSize(VALUE self, VALUE width, VALUE height);

  static VALUE setText(VALUE self, VALUE value);

  // Variables
  static VALUE getter_x(VALUE self);
  static VALUE getter_y(VALUE self);

  static VALUE setter_x(VALUE self, VALUE value);
  static VALUE setter_y(VALUE self, VALUE value);
};

}  // namespace ae