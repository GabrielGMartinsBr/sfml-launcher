#include "./AeButtonIntegrator.h"

#include <SFML/System/String.hpp>
#include <cassert>

#include "AppDefs.h"
#include "aeon/window/AeonButtonElement.h"
#include "aeon/window/AeonStyleSheet.h"
#include "aeon/window/integration/AeElementStyleParser.hpp"
#include "aeon/window/integration/AeStyleSheetIntegrator.h"
#include "integrator/Convert.hpp"

namespace ae {

using app::CStr;
using app::String;

VALUE AeButtonIntegrator::classObject;

// Integrate to ruby

void AeButtonIntegrator::integrate(VALUE aeonModule)
{
  classObject = rb_define_class_under(aeonModule, "Button", rb_cObject);
  rb_define_alloc_func(classObject, instanceAllocator);

  rb_define_method(classObject, "initialize", RUBY_METHOD_FUNC(initialize), -1);
  rb_define_method(classObject, "setPosition", RUBY_METHOD_FUNC(setPosition), 2);
  rb_define_method(classObject, "setSize", RUBY_METHOD_FUNC(setSize), 2);
  rb_define_method(classObject, "setText", RUBY_METHOD_FUNC(setText), 1);

  rb_define_method(classObject, "setStyle", RUBY_METHOD_FUNC(setStyleProp), 2);
  rb_define_method(classObject, "setStyleSheet", RUBY_METHOD_FUNC(setStyleSheet), -1);

  rb_define_method(classObject, "key", RUBY_METHOD_FUNC(getKey), 0);
  rb_define_method(classObject, "key=", RUBY_METHOD_FUNC(setKey), 1);
  rb_define_method(classObject, "getKey", RUBY_METHOD_FUNC(getKey), 0);
  rb_define_method(classObject, "setKey", RUBY_METHOD_FUNC(setKey), 1);

  rb_define_method(classObject, "x", RUBY_METHOD_FUNC(getter_x), 0);
  rb_define_method(classObject, "x=", RUBY_METHOD_FUNC(setter_x), 1);

  rb_define_method(classObject, "y", RUBY_METHOD_FUNC(getter_y), 0);
  rb_define_method(classObject, "y=", RUBY_METHOD_FUNC(setter_y), 1);

  rb_define_method(classObject, "width", RUBY_METHOD_FUNC(getter_width), 0);
  rb_define_method(classObject, "height", RUBY_METHOD_FUNC(getter_height), 0);

  // Methods
  rb_define_method(classObject, "flush", RUBY_METHOD_FUNC(flush), 0);

  rb_define_method(classObject, "isClicked", RUBY_METHOD_FUNC(isClicked), 0);
  rb_define_method(classObject, "isTriggered", RUBY_METHOD_FUNC(isTriggered), 0);
}

// Instance allocator

VALUE AeButtonIntegrator::instanceAllocator(VALUE instanceClass)
{
  return Data_Wrap_Struct(instanceClass, instanceMark, instanceFree, nullptr);
}

// Instance deallocator

void AeButtonIntegrator::instanceFree(void *ptr)
{
  delete static_cast<AeonButtonElement *>(ptr);
}

// Instance mark

void AeButtonIntegrator::instanceMark(void *ptr) { }

/*
  ⇩⇩⇩ Instance props and methods ⇩⇩⇩
*/

// Initialize

VALUE AeButtonIntegrator::initialize(int argc, VALUE *argv, VALUE self)
{
  AeonButtonElement *instance = new AeonButtonElement();

  DATA_PTR(self) = instance;
  instance->handleInitialize(self);

  return self;
}

// Set element size

VALUE AeButtonIntegrator::setSize(VALUE self, VALUE _width, VALUE _height)
{
  AeonButtonElement &inst = AeonIntegratorBase::getWrappedObject(self);

  float width = Convert::toCDouble2(_width);
  float height = Convert::toCDouble2(_height);

  inst.setSize(width, height);
  inst.setInstanceVar("@width", _width);
  inst.setInstanceVar("@height", _height);

  return Qnil;
}

// Set element position

VALUE AeButtonIntegrator::setPosition(VALUE self, VALUE rbX, VALUE rbY)
{
  AeonButtonElement &inst = AeonIntegratorBase::getWrappedObject(self);

  float x = Convert::toCDouble2(rbX);
  float y = Convert::toCDouble2(rbY);

  inst.setPosition(x, y);
  inst.setInstanceVar("@x", rbX);
  inst.setInstanceVar("@y", rbY);

  return Qnil;
}

VALUE AeButtonIntegrator::setStyleProp(VALUE self, VALUE propKey, VALUE value)
{
  AeonButtonElement &inst = AeonIntegratorBase::getWrappedObject(self);
  AeElementStyleParser::setStyleProp(inst, propKey, value);
  return Qnil;
}

VALUE AeButtonIntegrator::setStyleSheet(int argc, VALUE *argv, VALUE self)
{
  AeonButtonElement &inst = getWrappedObject(self);

  if (argc == 1) {
    AeonStyleSheet &style = AeStyleSheetIntegrator::getWrappedObject(argv[0]);
    inst.setStyle(style);
    return Qnil;
  }

  if (argc == 2) {
    CStr stateName = Convert::toCStr(argv[0]);
    AeonElementState state = AeElementStyleParser::parseElementState(stateName);
    if (state != AeonElementState::DEFAULT && state != AeonElementState::UNKNOW) {
      AeonStyleSheet &style = AeStyleSheetIntegrator::getWrappedObject(argv[1]);
      inst.setStateStyle(state, style);
      return Qnil;
    } else {
      return raiseException("Invalid Element state received on setStyleSheet method");
    }
  }

  return raiseException("setStyleSheet takes 1 or 2 arguments");
}

// Set text var

VALUE AeButtonIntegrator::setText(VALUE self, VALUE value)
{
  AeonButtonElement &inst = AeonIntegratorBase::getWrappedObject(self);
  const String &resValue = inst.setText(Convert::toCStr(value));
  inst.setInstanceVar("@text", resValue);
  return Qnil;
}

// Element key

VALUE AeButtonIntegrator::getKey(VALUE self)
{
  AeonButtonElement &inst = AeonIntegratorBase::getWrappedObject(self);
  return Convert::toRubyString(inst.getKey());
}

VALUE AeButtonIntegrator::setKey(VALUE self, VALUE value)
{
  AeonButtonElement &inst = AeonIntegratorBase::getWrappedObject(self);
  inst.setKey(Convert::toCStr(value));
  return value;
}

VALUE AeButtonIntegrator::getter_x(VALUE self)
{
  AeonButtonElement &inst = AeonIntegratorBase::getWrappedObject(self);
  VALUE value = inst.toRubyValue(inst.getBounds().x());
  return value;
}

VALUE AeButtonIntegrator::setter_x(VALUE self, VALUE value)
{
  AeonButtonElement &inst = AeonIntegratorBase::getWrappedObject(self);
  inst.x(Convert::toCDouble2(value));
  inst.setInstanceVar("@x", value);
  return value;
}

VALUE AeButtonIntegrator::getter_y(VALUE self)
{
  AeonButtonElement &inst = AeonIntegratorBase::getWrappedObject(self);
  VALUE value = inst.toRubyValue(inst.getBounds().y());
  return value;
}

VALUE AeButtonIntegrator::setter_y(VALUE self, VALUE value)
{
  AeonButtonElement &inst = AeonIntegratorBase::getWrappedObject(self);
  inst.y(Convert::toCDouble2(value));
  inst.setInstanceVar("@y", value);
  return value;
}

VALUE AeButtonIntegrator::getter_width(VALUE self)
{
  AeonButtonElement &inst = AeonIntegratorBase::getWrappedObject(self);
  return inst.toRubyValue(inst.getBounds().width());
}

VALUE AeButtonIntegrator::getter_height(VALUE self)
{
  AeonButtonElement &inst = AeonIntegratorBase::getWrappedObject(self);
  return inst.toRubyValue(inst.getBounds().height());
}

VALUE AeButtonIntegrator::flush(VALUE self)
{
  AeonButtonElement &inst = AeonIntegratorBase::getWrappedObject(self);
  inst.flush();
  return Qnil;
}

VALUE AeButtonIntegrator::isClicked(VALUE self)
{
  AeonButtonElement &inst = getWrappedObject(self);
  return Convert::toRubyBool(inst.isClicked());
}

VALUE AeButtonIntegrator::isTriggered(VALUE self)
{
  AeonButtonElement &inst = getWrappedObject(self);
  return Convert::toRubyBool(inst.isTriggered());
}

}  // namespace ae