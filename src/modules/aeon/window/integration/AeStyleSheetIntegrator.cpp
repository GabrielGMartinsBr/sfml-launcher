#include "./AeStyleSheetIntegrator.h"

#include "AppDefs.h"
#include "aeon/window/AeonStyleSheet.h"
#include "aeon/window/integration/AeElementStyleParser.hpp"
#include "integrator/Convert.hpp"

namespace ae {

using app::CStr;

VALUE AeStyleSheetIntegrator::classObject;

void AeStyleSheetIntegrator::integrate(VALUE aeonModule)
{
  classObject = rb_define_class_under(aeonModule, "StyleSheet", rb_cObject);
  rb_define_alloc_func(classObject, instanceAllocator);

  rb_define_method(classObject, "initialize", RUBY_METHOD_FUNC(initialize), 0);

  rb_define_method(classObject, "[]", RUBY_METHOD_FUNC(getter_prop), 1);
  rb_define_method(classObject, "[]=", RUBY_METHOD_FUNC(setter_prop), 2);
}

// Instance allocator

VALUE AeStyleSheetIntegrator::instanceAllocator(VALUE instanceClass)
{
  return Data_Wrap_Struct(instanceClass, instanceMark, instanceFree, nullptr);
}

// Instance deallocator

void AeStyleSheetIntegrator::instanceFree(void *ptr)
{
  //   wrapList().removeObject()
  delete static_cast<AeonStyleSheet *>(ptr);
}

// Instance mark

void AeStyleSheetIntegrator::instanceMark(void *ptr) { }

/*
  ⇩⇩⇩ Instance props and methods ⇩⇩⇩
*/

// Initialize

VALUE AeStyleSheetIntegrator::initialize(VALUE self)
{
  AeonStyleSheet *inst = new AeonStyleSheet();
  DATA_PTR(self) = inst;
  return self;
}

VALUE AeStyleSheetIntegrator::getter_prop(VALUE self, VALUE name)
{
  AeonStyleSheet &inst = getWrappedObject(self);
  return Qnil;
}

VALUE AeStyleSheetIntegrator::setter_prop(VALUE self, VALUE nameRb, VALUE value)
{
  AeonStyleSheet &inst = getWrappedObject(self);

  CStr name = Convert::toCStr(nameRb);

  AeElementStyleParser::setStyleProp(inst, name, value);

  return Qnil;
}
}  // namespace ae