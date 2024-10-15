#pragma once

#include <ruby.h>

namespace ae {

struct AeonModuleIntegrator {
  static VALUE aeonModule;

  static void integrate();

  static void method_update();
};
}  // namespace ae