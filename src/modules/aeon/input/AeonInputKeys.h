#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cstdint>

namespace ae {

using SfKey = sf::Keyboard::Key;
using SfMouse = sf::Mouse;

namespace InputCode {

enum Key {
  A = SfKey::A,
  B = SfKey::B,
  C = SfKey::C,
  D = SfKey::D,
  E = SfKey::E,
  F = SfKey::F,
  G = SfKey::G,
  H = SfKey::H,
  I = SfKey::I,
  J = SfKey::J,
  K = SfKey::K,
  L = SfKey::L,
  M = SfKey::M,
  N = SfKey::N,
  O = SfKey::O,
  P = SfKey::P,
  Q = SfKey::Q,
  R = SfKey::R,
  S = SfKey::S,
  T = SfKey::T,
  U = SfKey::U,
  V = SfKey::V,
  W = SfKey::W,
  X = SfKey::X,
  Y = SfKey::Y,
  Z = SfKey::Z,
  Num0 = SfKey::Num0,
  Num1 = SfKey::Num1,
  Num2 = SfKey::Num2,
  Num3 = SfKey::Num3,
  Num4 = SfKey::Num4,
  Num5 = SfKey::Num5,
  Num6 = SfKey::Num6,
  Num7 = SfKey::Num7,
  Num8 = SfKey::Num8,
  Num9 = SfKey::Num9,
  Escape = SfKey::Escape,
  LControl = SfKey::LControl,
  LShift = SfKey::LShift,
  LAlt = SfKey::LAlt,
  LSystem = SfKey::LSystem,
  RControl = SfKey::RControl,
  RShift = SfKey::RShift,
  RAlt = SfKey::RAlt,
  RSystem = SfKey::RSystem,
  Menu = SfKey::Menu,
  LBracket = SfKey::LBracket,
  RBracket = SfKey::RBracket,
  Semicolon = SfKey::Semicolon,
  Comma = SfKey::Comma,
  Period = SfKey::Period,
  Apostrophe = SfKey::Apostrophe,
  Slash = SfKey::Slash,
  Backslash = SfKey::Backslash,
  Grave = SfKey::Grave,
  Equal = SfKey::Equal,
  Hyphen = SfKey::Hyphen,
  Space = SfKey::Space,
  Enter = SfKey::Enter,
  Backspace = SfKey::Backspace,
  Tab = SfKey::Tab,
  PageUp = SfKey::PageUp,
  PageDown = SfKey::PageDown,
  End = SfKey::End,
  Home = SfKey::Home,
  Insert = SfKey::Insert,
  Delete = SfKey::Delete,
  Add = SfKey::Add,
  Subtract = SfKey::Subtract,
  Multiply = SfKey::Multiply,
  Divide = SfKey::Divide,
  Left = SfKey::Left,
  Right = SfKey::Right,
  Up = SfKey::Up,
  Down = SfKey::Down,
  Numpad0 = SfKey::Numpad0,
  Numpad1 = SfKey::Numpad1,
  Numpad2 = SfKey::Numpad2,
  Numpad3 = SfKey::Numpad3,
  Numpad4 = SfKey::Numpad4,
  Numpad5 = SfKey::Numpad5,
  Numpad6 = SfKey::Numpad6,
  Numpad7 = SfKey::Numpad7,
  Numpad8 = SfKey::Numpad8,
  Numpad9 = SfKey::Numpad9,
  F1 = SfKey::F1,
  F2 = SfKey::F2,
  F3 = SfKey::F3,
  F4 = SfKey::F4,
  F5 = SfKey::F5,
  F6 = SfKey::F6,
  F7 = SfKey::F7,
  F8 = SfKey::F8,
  F9 = SfKey::F9,
  F10 = SfKey::F10,
  F11 = SfKey::F11,
  F12 = SfKey::F12,
  F13 = SfKey::F13,
  F14 = SfKey::F14,
  F15 = SfKey::F15,
  Pause = SfKey::Pause,
  KeyCount = SfKey::KeyCount,
};

enum Mouse {
  MouseLeft = SfMouse::Left + SfKey::KeyCount,
  MouseRight = SfMouse::Right + SfKey::KeyCount,
  MouseMiddle = SfMouse::Middle + SfKey::KeyCount,
  MouseXButton1 = SfMouse::XButton1 + SfKey::KeyCount,
  MouseXButton2 = SfMouse::XButton2 + SfKey::KeyCount,
  MouseButtonCount = SfMouse::ButtonCount,
};

static constexpr int8_t Unknown = -1;
static constexpr int8_t Count = SfKey::KeyCount + SfMouse::ButtonCount;

}  // namespace InputCode

}  // namespace ae