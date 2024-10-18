#pragma once

#include <ruby.h>

#include "aeon/base/AeonIntegratorBase.hpp"
#include "aeon/window/AeonStyleSheet.h"

namespace ae {

struct AeStyleSheetIntegrator : public AeonIntegratorBase<AeonStyleSheet> {
  static VALUE classObject;

  static void integrate(VALUE aeonModule);

  static VALUE instanceAllocator(VALUE instanceClass);
  static void instanceFree(void *ptr);
  static void instanceMark(void *ptr);

  static VALUE initialize(VALUE self);

  static VALUE setter_prop(VALUE self, VALUE name, VALUE value);
  static VALUE getter_prop(VALUE self, VALUE name);
};

}  // namespace ae