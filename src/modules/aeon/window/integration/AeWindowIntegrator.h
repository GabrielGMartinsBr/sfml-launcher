#pragma once

#include <ruby.h>

#include "aeon/base/AeonIntegratorBase.hpp"
#include "aeon/window/AeonWindow.h"

namespace ae {

struct AeWindowIntegrator : public AeonIntegratorBase<AeonWindow> {
  static VALUE classObject;

  static void integrate(VALUE aeonModule);

  static VALUE instance_allocator(VALUE instanceClass);
  static void instance_free(void *ptr);
  static void instance_mark(void *ptr);

  static VALUE meth_initialize(int argc, VALUE *argv, VALUE self);

  static VALUE getHoverElementKey(VALUE self);
  static VALUE getFocusElementKey(VALUE self);
  static VALUE getClickElementKey(VALUE self);
  static VALUE getTriggerElementKey(VALUE self);

  static VALUE disposed(VALUE self);
  static VALUE dispose(VALUE self);
  
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

  static VALUE addElement(VALUE self, VALUE value);
};

}  // namespace ae