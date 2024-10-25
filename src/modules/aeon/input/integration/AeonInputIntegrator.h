#pragma once

#include <ruby.h>

#include <cstdint>

namespace ae {

struct AeonInputIntegrator {
  static VALUE classObject;

  static void integrate(VALUE aeonModule);

  static VALUE getMouseX(VALUE self);
  static VALUE getMouseY(VALUE self);

  static VALUE isPressed(VALUE self, VALUE key);
  static VALUE isTrigger(VALUE self, VALUE key);
  static VALUE isRepeat(VALUE self, VALUE key);

  static int8_t inputCodeFromValue(VALUE rbValue);
};

}  // namespace ae