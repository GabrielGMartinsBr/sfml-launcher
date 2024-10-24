#include "./AeonModuleIntegrator.h"

#include <ruby.h>

#include "aeon/input/integration/AeonInputIntegrator.h"
#include "aeon/socket/integration/AeonSocketIntegrator.h"
#include "aeon/window/integration/AeButtonIntegrator.h"
#include "aeon/window/integration/AeStyleSheetIntegrator.h"
#include "aeon/window/integration/AeTextBoxIntegrator.h"
#include "aeon/window/integration/AeWindowIntegrator.h"

namespace ae {

VALUE AeonModuleIntegrator::aeonModule;

void AeonModuleIntegrator::integrate()
{
  aeonModule = rb_define_module("Aeon");

  AeonInputIntegrator::integrate(aeonModule);
  AeWindowIntegrator::integrate(aeonModule);
  AeButtonIntegrator::integrate(aeonModule);
  AeTextBoxIntegrator::integrate(aeonModule);
  AeStyleSheetIntegrator::integrate(aeonModule);
  AeonSocketIntegrator::integrate(aeonModule);
}

}  // namespace ae