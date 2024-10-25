#pragma once

#include <ruby.h>

#include <cstdint>

namespace ae {

struct AeonInputIntegrator {
  static VALUE classObject;

  static void integrate(VALUE aeonModule);

  static int8_t inputCodeFromValue(VALUE rbValue);

  static VALUE isPressed(VALUE self, VALUE key);
  static VALUE isTrigger(VALUE self, VALUE key);
  static VALUE isRepeat(VALUE self, VALUE key);
};

}  // namespace ae