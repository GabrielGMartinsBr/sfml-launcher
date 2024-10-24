#include "aeon/input/integration/AeonInputIntegrator.h"

#include <SFML/Window/Keyboard.hpp>

#include "aeon/input/AeonInput.h"
#include "integrator/Convert.hpp"

namespace ae {

void AeonInputIntegrator::integrate(VALUE aeonModule)
{
  VALUE moduleObject = rb_define_module_under(aeonModule, "Input");

  rb_define_module_function(moduleObject, "press?", RUBY_METHOD_FUNC(isPressed), 1);
  rb_define_module_function(moduleObject, "trigger?", RUBY_METHOD_FUNC(isTrigger), 1);
  rb_define_module_function(moduleObject, "repeat?", RUBY_METHOD_FUNC(isRepeat), 1);

  rb_define_const(moduleObject, "Unknown", Convert::toRubyNumber(SfKey::Unknown));
  rb_define_const(moduleObject, "A", Convert::toRubyNumber(SfKey::A));
  rb_define_const(moduleObject, "B", Convert::toRubyNumber(SfKey::B));
  rb_define_const(moduleObject, "C", Convert::toRubyNumber(SfKey::C));
  rb_define_const(moduleObject, "D", Convert::toRubyNumber(SfKey::D));
  rb_define_const(moduleObject, "E", Convert::toRubyNumber(SfKey::E));
  rb_define_const(moduleObject, "F", Convert::toRubyNumber(SfKey::F));
  rb_define_const(moduleObject, "G", Convert::toRubyNumber(SfKey::G));
  rb_define_const(moduleObject, "H", Convert::toRubyNumber(SfKey::H));
  rb_define_const(moduleObject, "I", Convert::toRubyNumber(SfKey::I));
  rb_define_const(moduleObject, "J", Convert::toRubyNumber(SfKey::J));
  rb_define_const(moduleObject, "K", Convert::toRubyNumber(SfKey::K));
  rb_define_const(moduleObject, "L", Convert::toRubyNumber(SfKey::L));
  rb_define_const(moduleObject, "M", Convert::toRubyNumber(SfKey::M));
  rb_define_const(moduleObject, "N", Convert::toRubyNumber(SfKey::N));
  rb_define_const(moduleObject, "O", Convert::toRubyNumber(SfKey::O));
  rb_define_const(moduleObject, "P", Convert::toRubyNumber(SfKey::P));
  rb_define_const(moduleObject, "Q", Convert::toRubyNumber(SfKey::Q));
  rb_define_const(moduleObject, "R", Convert::toRubyNumber(SfKey::R));
  rb_define_const(moduleObject, "S", Convert::toRubyNumber(SfKey::S));
  rb_define_const(moduleObject, "T", Convert::toRubyNumber(SfKey::T));
  rb_define_const(moduleObject, "U", Convert::toRubyNumber(SfKey::U));
  rb_define_const(moduleObject, "V", Convert::toRubyNumber(SfKey::V));
  rb_define_const(moduleObject, "W", Convert::toRubyNumber(SfKey::W));
  rb_define_const(moduleObject, "X", Convert::toRubyNumber(SfKey::X));
  rb_define_const(moduleObject, "Y", Convert::toRubyNumber(SfKey::Y));
  rb_define_const(moduleObject, "Z", Convert::toRubyNumber(SfKey::Z));
  rb_define_const(moduleObject, "Num0", Convert::toRubyNumber(SfKey::Num0));
  rb_define_const(moduleObject, "Num1", Convert::toRubyNumber(SfKey::Num1));
  rb_define_const(moduleObject, "Num2", Convert::toRubyNumber(SfKey::Num2));
  rb_define_const(moduleObject, "Num3", Convert::toRubyNumber(SfKey::Num3));
  rb_define_const(moduleObject, "Num4", Convert::toRubyNumber(SfKey::Num4));
  rb_define_const(moduleObject, "Num5", Convert::toRubyNumber(SfKey::Num5));
  rb_define_const(moduleObject, "Num6", Convert::toRubyNumber(SfKey::Num6));
  rb_define_const(moduleObject, "Num7", Convert::toRubyNumber(SfKey::Num7));
  rb_define_const(moduleObject, "Num8", Convert::toRubyNumber(SfKey::Num8));
  rb_define_const(moduleObject, "Num9", Convert::toRubyNumber(SfKey::Num9));
  rb_define_const(moduleObject, "Escape", Convert::toRubyNumber(SfKey::Escape));
  rb_define_const(moduleObject, "LControl", Convert::toRubyNumber(SfKey::LControl));
  rb_define_const(moduleObject, "LShift", Convert::toRubyNumber(SfKey::LShift));
  rb_define_const(moduleObject, "LAlt", Convert::toRubyNumber(SfKey::LAlt));
  rb_define_const(moduleObject, "LSystem", Convert::toRubyNumber(SfKey::LSystem));
  rb_define_const(moduleObject, "RControl", Convert::toRubyNumber(SfKey::RControl));
  rb_define_const(moduleObject, "RShift", Convert::toRubyNumber(SfKey::RShift));
  rb_define_const(moduleObject, "RAlt", Convert::toRubyNumber(SfKey::RAlt));
  rb_define_const(moduleObject, "RSystem", Convert::toRubyNumber(SfKey::RSystem));
  rb_define_const(moduleObject, "Menu", Convert::toRubyNumber(SfKey::Menu));
  rb_define_const(moduleObject, "LBracket", Convert::toRubyNumber(SfKey::LBracket));
  rb_define_const(moduleObject, "RBracket", Convert::toRubyNumber(SfKey::RBracket));
  rb_define_const(moduleObject, "Semicolon", Convert::toRubyNumber(SfKey::Semicolon));
  rb_define_const(moduleObject, "Comma", Convert::toRubyNumber(SfKey::Comma));
  rb_define_const(moduleObject, "Period", Convert::toRubyNumber(SfKey::Period));
  rb_define_const(moduleObject, "Apostrophe", Convert::toRubyNumber(SfKey::Apostrophe));
  rb_define_const(moduleObject, "Slash", Convert::toRubyNumber(SfKey::Slash));
  rb_define_const(moduleObject, "Backslash", Convert::toRubyNumber(SfKey::Backslash));
  rb_define_const(moduleObject, "Grave", Convert::toRubyNumber(SfKey::Grave));
  rb_define_const(moduleObject, "Equal", Convert::toRubyNumber(SfKey::Equal));
  rb_define_const(moduleObject, "Hyphen", Convert::toRubyNumber(SfKey::Hyphen));
  rb_define_const(moduleObject, "Space", Convert::toRubyNumber(SfKey::Space));
  rb_define_const(moduleObject, "Enter", Convert::toRubyNumber(SfKey::Enter));
  rb_define_const(moduleObject, "Backspace", Convert::toRubyNumber(SfKey::Backspace));
  rb_define_const(moduleObject, "Tab", Convert::toRubyNumber(SfKey::Tab));
  rb_define_const(moduleObject, "PageUp", Convert::toRubyNumber(SfKey::PageUp));
  rb_define_const(moduleObject, "PageDown", Convert::toRubyNumber(SfKey::PageDown));
  rb_define_const(moduleObject, "End", Convert::toRubyNumber(SfKey::End));
  rb_define_const(moduleObject, "Home", Convert::toRubyNumber(SfKey::Home));
  rb_define_const(moduleObject, "Insert", Convert::toRubyNumber(SfKey::Insert));
  rb_define_const(moduleObject, "Delete", Convert::toRubyNumber(SfKey::Delete));
  rb_define_const(moduleObject, "Add", Convert::toRubyNumber(SfKey::Add));
  rb_define_const(moduleObject, "Subtract", Convert::toRubyNumber(SfKey::Subtract));
  rb_define_const(moduleObject, "Multiply", Convert::toRubyNumber(SfKey::Multiply));
  rb_define_const(moduleObject, "Divide", Convert::toRubyNumber(SfKey::Divide));
  rb_define_const(moduleObject, "Left", Convert::toRubyNumber(SfKey::Left));
  rb_define_const(moduleObject, "Right", Convert::toRubyNumber(SfKey::Right));
  rb_define_const(moduleObject, "Up", Convert::toRubyNumber(SfKey::Up));
  rb_define_const(moduleObject, "Down", Convert::toRubyNumber(SfKey::Down));
  rb_define_const(moduleObject, "Numpad0", Convert::toRubyNumber(SfKey::Numpad0));
  rb_define_const(moduleObject, "Numpad1", Convert::toRubyNumber(SfKey::Numpad1));
  rb_define_const(moduleObject, "Numpad2", Convert::toRubyNumber(SfKey::Numpad2));
  rb_define_const(moduleObject, "Numpad3", Convert::toRubyNumber(SfKey::Numpad3));
  rb_define_const(moduleObject, "Numpad4", Convert::toRubyNumber(SfKey::Numpad4));
  rb_define_const(moduleObject, "Numpad5", Convert::toRubyNumber(SfKey::Numpad5));
  rb_define_const(moduleObject, "Numpad6", Convert::toRubyNumber(SfKey::Numpad6));
  rb_define_const(moduleObject, "Numpad7", Convert::toRubyNumber(SfKey::Numpad7));
  rb_define_const(moduleObject, "Numpad8", Convert::toRubyNumber(SfKey::Numpad8));
  rb_define_const(moduleObject, "Numpad9", Convert::toRubyNumber(SfKey::Numpad9));
  rb_define_const(moduleObject, "F1", Convert::toRubyNumber(SfKey::F1));
  rb_define_const(moduleObject, "F2", Convert::toRubyNumber(SfKey::F2));
  rb_define_const(moduleObject, "F3", Convert::toRubyNumber(SfKey::F3));
  rb_define_const(moduleObject, "F4", Convert::toRubyNumber(SfKey::F4));
  rb_define_const(moduleObject, "F5", Convert::toRubyNumber(SfKey::F5));
  rb_define_const(moduleObject, "F6", Convert::toRubyNumber(SfKey::F6));
  rb_define_const(moduleObject, "F7", Convert::toRubyNumber(SfKey::F7));
  rb_define_const(moduleObject, "F8", Convert::toRubyNumber(SfKey::F8));
  rb_define_const(moduleObject, "F9", Convert::toRubyNumber(SfKey::F9));
  rb_define_const(moduleObject, "F10", Convert::toRubyNumber(SfKey::F10));
  rb_define_const(moduleObject, "F11", Convert::toRubyNumber(SfKey::F11));
  rb_define_const(moduleObject, "F12", Convert::toRubyNumber(SfKey::F12));
  rb_define_const(moduleObject, "F13", Convert::toRubyNumber(SfKey::F13));
  rb_define_const(moduleObject, "F14", Convert::toRubyNumber(SfKey::F14));
  rb_define_const(moduleObject, "F15", Convert::toRubyNumber(SfKey::F15));
  rb_define_const(moduleObject, "Pause", Convert::toRubyNumber(SfKey::Pause));
}

SfKey AeonInputIntegrator::keyFromValue(VALUE rbValue)
{
  int8_t value = Convert::toCInt(rbValue);
  return AeonInput::isValidKey(value) ? static_cast<SfKey>(value) : SfKey::Unknown;
}

VALUE AeonInputIntegrator::isPressed(VALUE self, VALUE key)
{
  return AeonInput::Instance().isPressed(
    keyFromValue(key)
  );
}

VALUE AeonInputIntegrator::isTrigger(VALUE self, VALUE key)
{
  return AeonInput::Instance().isTriggered(
    keyFromValue(key)
  );
}

VALUE AeonInputIntegrator::isRepeat(VALUE self, VALUE key)
{
  return AeonInput::Instance().isRepeated(
    keyFromValue(key)
  );
}

}  // namespace ae