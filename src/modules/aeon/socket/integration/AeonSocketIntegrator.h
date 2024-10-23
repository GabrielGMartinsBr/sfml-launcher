#pragma once

#include <ruby.h>

#include "./AeonSocketIntegrable.h"
#include "aeon/base/AeonIntegratorBase.hpp"

namespace ae {

struct AeonSocketIntegrator : public AeonIntegratorBase<AeonSocketIntegrable> {
  static VALUE classObject;

  static void integrate(VALUE aeonModule);

  static VALUE instanceAllocator(VALUE instanceClass);
  static void instanceFree(void *ptr);
  static void instanceMark(void *ptr);

  static VALUE initialize(VALUE self);

  static VALUE connect(VALUE self, VALUE host, VALUE port);
  static VALUE close(VALUE self);
  static VALUE disconnect(VALUE self);

  static VALUE sendMessage(VALUE self, VALUE message);

  static VALUE isConnected(VALUE self);

  static VALUE setConnectHandler(VALUE self, VALUE handler);
  static VALUE setMessageHandler(VALUE self, VALUE handler);
};

}  // namespace ae