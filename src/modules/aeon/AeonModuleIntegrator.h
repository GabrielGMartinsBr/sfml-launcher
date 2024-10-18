#pragma once

#include <ruby.h>

namespace ae {

struct AeonModuleIntegrator {
  static VALUE aeonModule;

  static void integrate();
};
}  // namespace ae