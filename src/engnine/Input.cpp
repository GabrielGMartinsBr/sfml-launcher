#include "engnine/Input.h"

#include <cassert>

namespace Eng {

/*
  ⇩⇩⇩ Static ⇩⇩⇩
*/
static Input* instance = nullptr;

void Input::Init()
{
  assert(!instance);
  instance = new Input();
}

Input& Input::Instance()
{
  assert(instance);
  return *instance;
}

void Input::Destroy()
{
  assert(instance);
  delete instance;
  instance = nullptr;
}

/*
  ⇩⇩⇩ Instance ⇩⇩⇩
*/

Input::Input() :
    aeonInput(AeonInput::Instance()),
    clock(AeonInput::Instance().getClock())
{
  dir4 = DirectionalCode::DIRECTIONAL_NONE;
  std::memset(currentKeys, 0, KEY_COUNT);
  std::memset(previousKeys, 0, KEY_COUNT);
  std::memset(pressKeys, 0, KEY_COUNT);
  std::memset(repeatTs, 0, KEY_COUNT);
}

void Input::handleKeyPressed(const SfKeyEvent& key)
{
  KeyCode keyCode = mapKey(key.code);
  if (keyCode == KeyCode::UNKNOW) return;
  currentKeys[keyCode] = true;
}

void Input::handleKeyRelease(const SfKeyEvent& key)
{
  KeyCode keyCode = mapKey(key.code);
  if (keyCode != KeyCode::UNKNOW) {
    currentKeys[keyCode] = false;
  }
}

void Input::update()
{
  std::memcpy(previousKeys, pressKeys, KEY_COUNT);
  std::memcpy(pressKeys, currentKeys, KEY_COUNT);

  updateDirectional();
}

bool Input::isPressed(KeyCode key)
{
  return pressKeys[key];
}

bool Input::isTriggered(KeyCode key)
{
  return pressKeys[key] && !previousKeys[key];
}

bool Input::isRepeated(KeyCode key)
{
  if (key == KeyCode::UNKNOW || !pressKeys[key]) {
    return false;
  }

  const UInt timestamp = clock.getElapsedTime().asMilliseconds();

  if (!previousKeys[key]) {
    repeatTs[key] = timestamp;
    return true;
  }

  UInt elapsedTime = timestamp - repeatTs[key];

  if (elapsedTime > repeatDelay) {
    UInt timeSinceRepeat = elapsedTime - repeatDelay;
    if (timeSinceRepeat >= repeatInterval) {
      repeatTs[key] = timestamp - repeatDelay;
      return true;
    }
  }

  return false;
}

bool Input::isValidKey(int num)
{
  return num > -1 && num < KEY_COUNT;
}

Input::DirectionalCode Input::getDir4()
{
  return dir4;
}

/*
  ⇩⇩⇩ Private ⇩⇩⇩
*/

void Input::updateDirectional()
{
  if (currentKeys[KeyCode::UP] && dir4 != DirectionalCode::DIRECTIONAL_DOWN) {
    dir4 = DirectionalCode::DIRECTIONAL_UP;
    return;
  } else if (currentKeys[KeyCode::DOWN] && dir4 != DirectionalCode::DIRECTIONAL_UP) {
    dir4 = DirectionalCode::DIRECTIONAL_DOWN;
    return;
  }

  if (currentKeys[KeyCode::LEFT] && dir4 != DirectionalCode::DIRECTIONAL_RIGHT) {
    dir4 = DirectionalCode::DIRECTIONAL_LEFT;
    return;
  } else if (currentKeys[KeyCode::RIGHT] && dir4 != DirectionalCode::DIRECTIONAL_LEFT) {
    dir4 = DirectionalCode::DIRECTIONAL_RIGHT;
    return;
  }

  dir4 = DirectionalCode::DIRECTIONAL_NONE;
}

Input::KeyCode Input::mapKey(const SfKey& key) const
{
  switch (key) {
    case sf::Keyboard::Down: {
      return KeyCode::DOWN;
    }
    case sf::Keyboard::Left: {
      return KeyCode::LEFT;
    }
    case sf::Keyboard::Right: {
      return KeyCode::RIGHT;
    }
    case sf::Keyboard::Up: {
      return KeyCode::UP;
    }
    case sf::Keyboard::Z:
      return KeyCode::A;
    case sf::Keyboard::Escape:
    case sf::Keyboard::X: {
      return KeyCode::B;
    }
    case sf::Keyboard::Enter:
    case sf::Keyboard::Space:
    case sf::Keyboard::C: {
      return KeyCode::C;
    }
    case sf::Keyboard::A: {
      return KeyCode::X;
    }
    case sf::Keyboard::S: {
      return KeyCode::Y;
    }
    case sf::Keyboard::D: {
      return KeyCode::Z;
    }
    case sf::Keyboard::Q: {
      return KeyCode::L;
    }
    case sf::Keyboard::W: {
      return KeyCode::R;
    }
    case sf::Keyboard::LShift:
    case sf::Keyboard::RShift: {
      return KeyCode::SHIFT;
    }
    case sf::Keyboard::LControl:
    case sf::Keyboard::RControl: {
      return KeyCode::CTRL;
    }
    case sf::Keyboard::LAlt:
    case sf::Keyboard::RAlt: {
      return KeyCode::ALT;
    }
    case sf::Keyboard::F5: {
      return KeyCode::F5;
    }
    case sf::Keyboard::F6: {
      return KeyCode::F6;
    }
    case sf::Keyboard::F7: {
      return KeyCode::F7;
    }
    case sf::Keyboard::F8: {
      return KeyCode::F8;
    }
    case sf::Keyboard::F9: {
      return KeyCode::F9;
    }
    default:
      return KeyCode::UNKNOW;
      break;
  }
}

}  // namespace Eng