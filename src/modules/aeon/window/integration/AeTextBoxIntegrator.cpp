#include "./AeTextBoxIntegrator.h"

#include <SFML/System/String.hpp>
#include <cassert>

#include "AppDefs.h"
#include "StringUtils.hpp"
#include "aeon/enums/AeonElementState.h"
#include "aeon/window/AeonStyleSheet.h"
#include "aeon/window/AeonTextBoxElement.h"
#include "aeon/window/integration/AeElementStyleParser.hpp"
#include "aeon/window/integration/AeStyleSheetIntegrator.h"
#include "integrator/Convert.hpp"

namespace ae {

using app::CStr;
using app::String;

VALUE AeTextBoxIntegrator::classObject;

// Integrate to ruby

void AeTextBoxIntegrator::integrate(VALUE aeonModule)
{
  classObject = rb_define_class_under(aeonModule, "TextBox", rb_cObject);
  rb_define_alloc_func(classObject, instanceAllocator);

  rb_define_method(classObject, "initialize", RUBY_METHOD_FUNC(initialize), -1);
  rb_define_method(classObject, "setPosition", RUBY_METHOD_FUNC(setPosition), 2);
  rb_define_method(classObject, "setSize", RUBY_METHOD_FUNC(setSize), 2);
  rb_define_method(classObject, "setStyle", RUBY_METHOD_FUNC(setStyleProp), 2);

  rb_define_method(classObject, "setStyleSheet", RUBY_METHOD_FUNC(setStyleSheet), -1);

  rb_define_method(classObject, "setValue", RUBY_METHOD_FUNC(setValue), 1);
  rb_define_method(classObject, "setIsPassword", RUBY_METHOD_FUNC(setIsPassword), 1);

  rb_define_method(classObject, "x", RUBY_METHOD_FUNC(getter_x), 0);
  rb_define_method(classObject, "x=", RUBY_METHOD_FUNC(setter_x), 1);

  rb_define_method(classObject, "y", RUBY_METHOD_FUNC(getter_y), 0);
  rb_define_method(classObject, "y=", RUBY_METHOD_FUNC(setter_y), 1);

  rb_define_method(classObject, "width", RUBY_METHOD_FUNC(getter_width), 0);
  rb_define_method(classObject, "height", RUBY_METHOD_FUNC(getter_height), 0);

  // Methods
  rb_define_method(classObject, "flush", RUBY_METHOD_FUNC(flush), 0);
}

// Instance allocator

VALUE AeTextBoxIntegrator::instanceAllocator(VALUE instanceClass)
{
  return Data_Wrap_Struct(instanceClass, instanceMark, instanceFree, nullptr);
}

// Instance deallocator

void AeTextBoxIntegrator::instanceFree(void *ptr)
{
  delete static_cast<AeonTextBoxElement *>(ptr);
}

// Instance mark

void AeTextBoxIntegrator::instanceMark(void *ptr) { }

/*
  ⇩⇩⇩ Instance props and methods ⇩⇩⇩
*/

// Initialize

VALUE AeTextBoxIntegrator::initialize(int argc, VALUE *argv, VALUE self)
{
  AeonTextBoxElement *instance = new AeonTextBoxElement();

  DATA_PTR(self) = instance;
  instance->handleInitialize(self);

  return self;
}

// Set element size

VALUE AeTextBoxIntegrator::setSize(VALUE self, VALUE _width, VALUE _height)
{
  AeonTextBoxElement &inst = AeonIntegratorBase::getWrappedObject(self);

  float width = Convert::toCDouble2(_width);
  float height = Convert::toCDouble2(_height);

  inst.setSize(width, height);
  inst.setInstanceVar("@width", _width);
  inst.setInstanceVar("@height", _height);

  return Qnil;
}

// Set element position

VALUE AeTextBoxIntegrator::setPosition(VALUE self, VALUE rbX, VALUE rbY)
{
  AeonTextBoxElement &inst = AeonIntegratorBase::getWrappedObject(self);

  float x = Convert::toCDouble2(rbX);
  float y = Convert::toCDouble2(rbY);

  inst.setPosition(x, y);
  inst.setInstanceVar("@x", rbX);
  inst.setInstanceVar("@y", rbY);

  return Qnil;
}

VALUE AeTextBoxIntegrator::setStyleSheet(int argc, VALUE *argv, VALUE self)
{
  AeonTextBoxElement &inst = getWrappedObject(self);

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

VALUE AeTextBoxIntegrator::setStyleProp(VALUE self, VALUE propKey, VALUE value)
{
  AeonTextBoxElement &inst = AeonIntegratorBase::getWrappedObject(self);
  AeElementStyleParser::setStyleProp(inst, propKey, value);
  return Qnil;
}

// Set text box value

VALUE AeTextBoxIntegrator::getValue(VALUE self)
{
  AeonTextBoxElement &inst = getWrappedObject(self);
  const sf::String &rawStr = inst.getValue();
  app::String str  = StringUtils::toUtf8(rawStr.toWideString());
  return Convert::toRubyString(str);
}

VALUE AeTextBoxIntegrator::setValue(VALUE self, VALUE value)
{
  AeonTextBoxElement &inst = AeonIntegratorBase::getWrappedObject(self);
  CStr rawStr = Convert::toCStr(value);
  const String &resValue = inst.setValue(StringUtils::fromUtf8(rawStr));
  inst.setInstanceVar("@value", resValue);
  return Qnil;
}

VALUE AeTextBoxIntegrator::setIsPassword(VALUE self, VALUE value)
{
  AeonTextBoxElement &inst = AeonIntegratorBase::getWrappedObject(self);
  bool isPassword = Convert::toCBool(value);
  inst.setIsPassword(isPassword);
  inst.setInstanceVar("@isPassword", value);
  return Qnil;
}

VALUE AeTextBoxIntegrator::getter_x(VALUE self)
{
  AeonTextBoxElement &inst = AeonIntegratorBase::getWrappedObject(self);
  VALUE value = inst.toRubyValue(inst.getBounds().x());
  return value;
}

VALUE AeTextBoxIntegrator::setter_x(VALUE self, VALUE value)
{
  AeonTextBoxElement &inst = AeonIntegratorBase::getWrappedObject(self);
  inst.x(Convert::toCDouble2(value));
  inst.setInstanceVar("@x", value);
  return value;
}

VALUE AeTextBoxIntegrator::getter_y(VALUE self)
{
  AeonTextBoxElement &inst = AeonIntegratorBase::getWrappedObject(self);
  VALUE value = inst.toRubyValue(inst.getBounds().y());
  return value;
}

VALUE AeTextBoxIntegrator::setter_y(VALUE self, VALUE value)
{
  AeonTextBoxElement &inst = AeonIntegratorBase::getWrappedObject(self);
  inst.y(Convert::toCDouble2(value));
  inst.setInstanceVar("@y", value);
  return value;
}

VALUE AeTextBoxIntegrator::getter_width(VALUE self)
{
  AeonTextBoxElement &inst = AeonIntegratorBase::getWrappedObject(self);
  return inst.toRubyValue(inst.getBounds().width());
}

VALUE AeTextBoxIntegrator::getter_height(VALUE self)
{
  AeonTextBoxElement &inst = AeonIntegratorBase::getWrappedObject(self);
  return inst.toRubyValue(inst.getBounds().height());
}

VALUE AeTextBoxIntegrator::flush(VALUE self)
{
  AeonTextBoxElement &inst = AeonIntegratorBase::getWrappedObject(self);
  inst.flush();
  return Qnil;
}

}  // namespace ae