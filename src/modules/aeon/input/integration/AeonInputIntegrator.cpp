#include "aeon/input/integration/AeonInputIntegrator.h"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "aeon/input/AeonInput.h"
#include "aeon/input/AeonInputKeys.h"
#include "integrator/Convert.hpp"

namespace ae {

void AeonInputIntegrator::integrate(VALUE aeonModule)
{
  VALUE moduleObject = rb_define_module_under(aeonModule, "Input");

  rb_define_module_function(moduleObject, "mouse_x", RUBY_METHOD_FUNC(getMouseX), 0);
  rb_define_module_function(moduleObject, "mouse_y", RUBY_METHOD_FUNC(getMouseY), 0);

  rb_define_module_function(moduleObject, "press?", RUBY_METHOD_FUNC(isPressed), 1);
  rb_define_module_function(moduleObject, "trigger?", RUBY_METHOD_FUNC(isTrigger), 1);
  rb_define_module_function(moduleObject, "repeat?", RUBY_METHOD_FUNC(isRepeat), 1);

  rb_define_const(moduleObject, "Mouse_Left", Convert::toRubyNumber(InputCode::MouseLeft));
  rb_define_const(moduleObject, "Mouse_Right", Convert::toRubyNumber(InputCode::MouseRight));
  rb_define_const(moduleObject, "Mouse_Middle", Convert::toRubyNumber(InputCode::MouseMiddle));
  rb_define_const(moduleObject, "Mouse_XButton1", Convert::toRubyNumber(InputCode::MouseXButton1));
  rb_define_const(moduleObject, "Mouse_XButton2", Convert::toRubyNumber(InputCode::MouseXButton2));

  rb_define_const(moduleObject, "Unknown", Convert::toRubyNumber(InputCode::Unknown));
  rb_define_const(moduleObject, "A", Convert::toRubyNumber(InputCode::A));
  rb_define_const(moduleObject, "B", Convert::toRubyNumber(InputCode::B));
  rb_define_const(moduleObject, "C", Convert::toRubyNumber(InputCode::C));
  rb_define_const(moduleObject, "D", Convert::toRubyNumber(InputCode::D));
  rb_define_const(moduleObject, "E", Convert::toRubyNumber(InputCode::E));
  rb_define_const(moduleObject, "F", Convert::toRubyNumber(InputCode::F));
  rb_define_const(moduleObject, "G", Convert::toRubyNumber(InputCode::G));
  rb_define_const(moduleObject, "H", Convert::toRubyNumber(InputCode::H));
  rb_define_const(moduleObject, "I", Convert::toRubyNumber(InputCode::I));
  rb_define_const(moduleObject, "J", Convert::toRubyNumber(InputCode::J));
  rb_define_const(moduleObject, "K", Convert::toRubyNumber(InputCode::K));
  rb_define_const(moduleObject, "L", Convert::toRubyNumber(InputCode::L));
  rb_define_const(moduleObject, "M", Convert::toRubyNumber(InputCode::M));
  rb_define_const(moduleObject, "N", Convert::toRubyNumber(InputCode::N));
  rb_define_const(moduleObject, "O", Convert::toRubyNumber(InputCode::O));
  rb_define_const(moduleObject, "P", Convert::toRubyNumber(InputCode::P));
  rb_define_const(moduleObject, "Q", Convert::toRubyNumber(InputCode::Q));
  rb_define_const(moduleObject, "R", Convert::toRubyNumber(InputCode::R));
  rb_define_const(moduleObject, "S", Convert::toRubyNumber(InputCode::S));
  rb_define_const(moduleObject, "T", Convert::toRubyNumber(InputCode::T));
  rb_define_const(moduleObject, "U", Convert::toRubyNumber(InputCode::U));
  rb_define_const(moduleObject, "V", Convert::toRubyNumber(InputCode::V));
  rb_define_const(moduleObject, "W", Convert::toRubyNumber(InputCode::W));
  rb_define_const(moduleObject, "X", Convert::toRubyNumber(InputCode::X));
  rb_define_const(moduleObject, "Y", Convert::toRubyNumber(InputCode::Y));
  rb_define_const(moduleObject, "Z", Convert::toRubyNumber(InputCode::Z));
  rb_define_const(moduleObject, "Num0", Convert::toRubyNumber(InputCode::Num0));
  rb_define_const(moduleObject, "Num1", Convert::toRubyNumber(InputCode::Num1));
  rb_define_const(moduleObject, "Num2", Convert::toRubyNumber(InputCode::Num2));
  rb_define_const(moduleObject, "Num3", Convert::toRubyNumber(InputCode::Num3));
  rb_define_const(moduleObject, "Num4", Convert::toRubyNumber(InputCode::Num4));
  rb_define_const(moduleObject, "Num5", Convert::toRubyNumber(InputCode::Num5));
  rb_define_const(moduleObject, "Num6", Convert::toRubyNumber(InputCode::Num6));
  rb_define_const(moduleObject, "Num7", Convert::toRubyNumber(InputCode::Num7));
  rb_define_const(moduleObject, "Num8", Convert::toRubyNumber(InputCode::Num8));
  rb_define_const(moduleObject, "Num9", Convert::toRubyNumber(InputCode::Num9));
  rb_define_const(moduleObject, "Escape", Convert::toRubyNumber(InputCode::Escape));
  rb_define_const(moduleObject, "LControl", Convert::toRubyNumber(InputCode::LControl));
  rb_define_const(moduleObject, "LShift", Convert::toRubyNumber(InputCode::LShift));
  rb_define_const(moduleObject, "LAlt", Convert::toRubyNumber(InputCode::LAlt));
  rb_define_const(moduleObject, "LSystem", Convert::toRubyNumber(InputCode::LSystem));
  rb_define_const(moduleObject, "RControl", Convert::toRubyNumber(InputCode::RControl));
  rb_define_const(moduleObject, "RShift", Convert::toRubyNumber(InputCode::RShift));
  rb_define_const(moduleObject, "RAlt", Convert::toRubyNumber(InputCode::RAlt));
  rb_define_const(moduleObject, "RSystem", Convert::toRubyNumber(InputCode::RSystem));
  rb_define_const(moduleObject, "Menu", Convert::toRubyNumber(InputCode::Menu));
  rb_define_const(moduleObject, "LBracket", Convert::toRubyNumber(InputCode::LBracket));
  rb_define_const(moduleObject, "RBracket", Convert::toRubyNumber(InputCode::RBracket));
  rb_define_const(moduleObject, "Semicolon", Convert::toRubyNumber(InputCode::Semicolon));
  rb_define_const(moduleObject, "Comma", Convert::toRubyNumber(InputCode::Comma));
  rb_define_const(moduleObject, "Period", Convert::toRubyNumber(InputCode::Period));
  rb_define_const(moduleObject, "Apostrophe", Convert::toRubyNumber(InputCode::Apostrophe));
  rb_define_const(moduleObject, "Slash", Convert::toRubyNumber(InputCode::Slash));
  rb_define_const(moduleObject, "Backslash", Convert::toRubyNumber(InputCode::Backslash));
  rb_define_const(moduleObject, "Grave", Convert::toRubyNumber(InputCode::Grave));
  rb_define_const(moduleObject, "Equal", Convert::toRubyNumber(InputCode::Equal));
  rb_define_const(moduleObject, "Hyphen", Convert::toRubyNumber(InputCode::Hyphen));
  rb_define_const(moduleObject, "Space", Convert::toRubyNumber(InputCode::Space));
  rb_define_const(moduleObject, "Enter", Convert::toRubyNumber(InputCode::Enter));
  rb_define_const(moduleObject, "Backspace", Convert::toRubyNumber(InputCode::Backspace));
  rb_define_const(moduleObject, "Tab", Convert::toRubyNumber(InputCode::Tab));
  rb_define_const(moduleObject, "PageUp", Convert::toRubyNumber(InputCode::PageUp));
  rb_define_const(moduleObject, "PageDown", Convert::toRubyNumber(InputCode::PageDown));
  rb_define_const(moduleObject, "End", Convert::toRubyNumber(InputCode::End));
  rb_define_const(moduleObject, "Home", Convert::toRubyNumber(InputCode::Home));
  rb_define_const(moduleObject, "Insert", Convert::toRubyNumber(InputCode::Insert));
  rb_define_const(moduleObject, "Delete", Convert::toRubyNumber(InputCode::Delete));
  rb_define_const(moduleObject, "Add", Convert::toRubyNumber(InputCode::Add));
  rb_define_const(moduleObject, "Subtract", Convert::toRubyNumber(InputCode::Subtract));
  rb_define_const(moduleObject, "Multiply", Convert::toRubyNumber(InputCode::Multiply));
  rb_define_const(moduleObject, "Divide", Convert::toRubyNumber(InputCode::Divide));
  rb_define_const(moduleObject, "Left", Convert::toRubyNumber(InputCode::Left));
  rb_define_const(moduleObject, "Right", Convert::toRubyNumber(InputCode::Right));
  rb_define_const(moduleObject, "Up", Convert::toRubyNumber(InputCode::Up));
  rb_define_const(moduleObject, "Down", Convert::toRubyNumber(InputCode::Down));
  rb_define_const(moduleObject, "Numpad0", Convert::toRubyNumber(InputCode::Numpad0));
  rb_define_const(moduleObject, "Numpad1", Convert::toRubyNumber(InputCode::Numpad1));
  rb_define_const(moduleObject, "Numpad2", Convert::toRubyNumber(InputCode::Numpad2));
  rb_define_const(moduleObject, "Numpad3", Convert::toRubyNumber(InputCode::Numpad3));
  rb_define_const(moduleObject, "Numpad4", Convert::toRubyNumber(InputCode::Numpad4));
  rb_define_const(moduleObject, "Numpad5", Convert::toRubyNumber(InputCode::Numpad5));
  rb_define_const(moduleObject, "Numpad6", Convert::toRubyNumber(InputCode::Numpad6));
  rb_define_const(moduleObject, "Numpad7", Convert::toRubyNumber(InputCode::Numpad7));
  rb_define_const(moduleObject, "Numpad8", Convert::toRubyNumber(InputCode::Numpad8));
  rb_define_const(moduleObject, "Numpad9", Convert::toRubyNumber(InputCode::Numpad9));
  rb_define_const(moduleObject, "F1", Convert::toRubyNumber(InputCode::F1));
  rb_define_const(moduleObject, "F2", Convert::toRubyNumber(InputCode::F2));
  rb_define_const(moduleObject, "F3", Convert::toRubyNumber(InputCode::F3));
  rb_define_const(moduleObject, "F4", Convert::toRubyNumber(InputCode::F4));
  rb_define_const(moduleObject, "F5", Convert::toRubyNumber(InputCode::F5));
  rb_define_const(moduleObject, "F6", Convert::toRubyNumber(InputCode::F6));
  rb_define_const(moduleObject, "F7", Convert::toRubyNumber(InputCode::F7));
  rb_define_const(moduleObject, "F8", Convert::toRubyNumber(InputCode::F8));
  rb_define_const(moduleObject, "F9", Convert::toRubyNumber(InputCode::F9));
  rb_define_const(moduleObject, "F10", Convert::toRubyNumber(InputCode::F10));
  rb_define_const(moduleObject, "F11", Convert::toRubyNumber(InputCode::F11));
  rb_define_const(moduleObject, "F12", Convert::toRubyNumber(InputCode::F12));
  rb_define_const(moduleObject, "F13", Convert::toRubyNumber(InputCode::F13));
  rb_define_const(moduleObject, "F14", Convert::toRubyNumber(InputCode::F14));
  rb_define_const(moduleObject, "F15", Convert::toRubyNumber(InputCode::F15));
  rb_define_const(moduleObject, "Pause", Convert::toRubyNumber(InputCode::Pause));
}

VALUE AeonInputIntegrator::getMouseX(VALUE self)
{
  return Convert::toRubyNumber(
    AeonInput::Instance().getMousePosition().x
  );
}

VALUE AeonInputIntegrator::getMouseY(VALUE self)
{
  return Convert::toRubyNumber(
    AeonInput::Instance().getMousePosition().y
  );
}

VALUE AeonInputIntegrator::isPressed(VALUE self, VALUE key)
{
  return Convert::toRubyBool(
    AeonInput::Instance().isPressed(
      inputCodeFromValue(key)
    )
  );
}

VALUE AeonInputIntegrator::isTrigger(VALUE self, VALUE key)
{
  return Convert::toRubyBool(
    AeonInput::Instance().isTriggered(
      inputCodeFromValue(key)
    )
  );
}

VALUE AeonInputIntegrator::isRepeat(VALUE self, VALUE key)
{
  return Convert::toRubyBool(
    AeonInput::Instance().isRepeated(
      inputCodeFromValue(key)
    )
  );
}

int8_t AeonInputIntegrator::inputCodeFromValue(VALUE rbValue)
{
  int8_t value = Convert::toCInt(rbValue);
  return AeonInput::isValidInputCode(value) ? value : InputCode::Unknown;
}

}  // namespace ae