#pragma once

#include <ruby.h>

#include "modules/aeon/window/AeonWindow.h"

namespace ae {

struct AeWindowIntegrator {
  static VALUE windowClass;

  static void integrate(VALUE aeonModule);

  static AeonWindow *getObjectValue(VALUE rbObj);

  static VALUE meth_initialize(int argc, VALUE *argv, VALUE self);

  static VALUE getter_windowSkin(VALUE self);
  static VALUE setter_windowSkin(VALUE self, VALUE value);

  static VALUE getter_contents(VALUE self);
  static VALUE setter_contents(VALUE self, VALUE value);

  static VALUE getter_x(VALUE self);
  static VALUE setter_x(VALUE self, VALUE value);

  static VALUE getter_y(VALUE self);
  static VALUE setter_y(VALUE self, VALUE value);

  static VALUE getter_width(VALUE self);
  static VALUE setter_width(VALUE self, VALUE value);

  static VALUE getter_height(VALUE self);
  static VALUE setter_height(VALUE self, VALUE value);
};

}  // namespace ae