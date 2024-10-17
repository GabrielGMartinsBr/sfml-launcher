#include "./AeButtonIntegrator.h"

#include <SFML/System/String.hpp>
#include <cassert>

#include "AppDefs.h"
#include "aeon/toolkit/ColorParser.hpp"
#include "aeon/window/AeonButtonElement.h"
#include "aeon/window/AeonStyleSheet.h"
#include "integrator/Convert.hpp"

namespace ae {

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

  rb_define_method(classObject, "x", RUBY_METHOD_FUNC(getter_x), 0);
  rb_define_method(classObject, "x=", RUBY_METHOD_FUNC(setter_x), 1);

  rb_define_method(classObject, "y", RUBY_METHOD_FUNC(getter_y), 0);
  rb_define_method(classObject, "y=", RUBY_METHOD_FUNC(setter_y), 1);
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
  ElementBounds bounds(32, 32, 160, 32);
  AeonStyleSheet style;

  AeonButtonElement *instance = new AeonButtonElement(bounds, style);

  DATA_PTR(self) = instance;
  instance->handleInitialize(self);

  AeonPartialStyleSheet hoverStyle;
  hoverStyle.borderColor = ColorParser::hexToNrgssColor("#fa4");
  instance->setStateStyle(AeonElementState::HOVER, hoverStyle);

  AeonPartialStyleSheet clickedStyle;
  // clickedStyle.borderColor = ColorParser::hexToNrgssColor("#f30");
  clickedStyle.bgColor = ColorParser::hexToNrgssColor("#fdd");
  instance->setStateStyle(AeonElementState::CLICKED, clickedStyle);

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

// Set text var

VALUE AeButtonIntegrator::setText(VALUE self, VALUE value)
{
  AeonButtonElement &inst = AeonIntegratorBase::getWrappedObject(self);
  const String &resValue = inst.setText(Convert::toCStr(value));
  inst.setInstanceVar("@text", resValue);
  return Qnil;
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
  inst.y(Convert::toCDouble2(value));
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

}  // namespace ae