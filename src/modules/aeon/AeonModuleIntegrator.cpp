#include "./AeonModuleIntegrator.h"

#include <ruby.h>

#include "Log.hpp"
#include "aeon/window/integration/AeButtonIntegrator.h"
#include "aeon/window/integration/AeWindowIntegrator.h"

namespace ae {

VALUE AeonModuleIntegrator::aeonModule;

void AeonModuleIntegrator::integrate()
{
  aeonModule = rb_define_module("Aeon");

  AeWindowIntegrator::integrate(aeonModule);
  AeButtonIntegrator::integrate(aeonModule);

  rb_define_module_function(aeonModule, "update", RUBY_METHOD_FUNC(AeonModuleIntegrator::method_update), 0);
}

void AeonModuleIntegrator::method_update()
{
  Log::out() << "UPDATE AEON!";
}

}  // namespace ae