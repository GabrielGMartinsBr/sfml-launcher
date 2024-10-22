#pragma once

#include <ruby.h>

#include "aeon/base/AeonIntegratorBase.hpp"
#include "aeon/socket/AeonSocket.hpp"

namespace ae {

struct AeonSocketIntegrator : public AeonIntegratorBase<AeonSocket> {
  static VALUE classObject;

  static void integrate(VALUE aeonModule);

  static VALUE instanceAllocator(VALUE instanceClass);
  static void instanceFree(void *ptr);
  static void instanceMark(void *ptr);

  static VALUE initialize(VALUE self);

  static VALUE connect(VALUE self, VALUE host, VALUE port);
  static VALUE disconnect(VALUE self);

  static VALUE sendMessage(VALUE self, VALUE message);

  static VALUE pollMessages(VALUE self);

  static VALUE setOnMessageCallback(VALUE self, VALUE callback);

  static VALUE isConnected(VALUE self);
};

}  // namespace ae