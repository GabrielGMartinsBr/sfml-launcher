#pragma once

#include <ruby.h>

#include "aeon/base/AeonIntegratorBase.hpp"
#include "aeon/window/AeonTextBoxElement.h"

namespace ae {

struct AeTextBoxIntegrator : public AeonIntegratorBase<AeonTextBoxElement> {
  static VALUE classObject;

  static void integrate(VALUE aeonModule);

  static VALUE instanceAllocator(VALUE instanceClass);
  static void instanceFree(void *ptr);
  static void instanceMark(void *ptr);

  static VALUE initialize(int argc, VALUE *argv, VALUE self);

  static VALUE setPosition(VALUE self, VALUE x, VALUE y);
  static VALUE setSize(VALUE self, VALUE width, VALUE height);

  static VALUE setStyleProp(VALUE self, VALUE propKey, VALUE value);
  static VALUE setStyleSheet(int argc, VALUE *argv, VALUE self);

  static VALUE setValue(VALUE self, VALUE value);
  static VALUE setIsPassword(VALUE self, VALUE value);

  // Variables
  static VALUE getter_x(VALUE self);
  static VALUE getter_y(VALUE self);
  static VALUE getter_width(VALUE self);
  static VALUE getter_height(VALUE self);

  static VALUE setter_x(VALUE self, VALUE value);
  static VALUE setter_y(VALUE self, VALUE value);

  // Methods

  static VALUE flush(VALUE self);
};
}  // namespace ae