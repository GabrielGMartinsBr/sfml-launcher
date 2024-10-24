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

Input::Input()
{
  for (int i = 1; i < InputCode::NUM_KEYS; i++) {
    keyTimes[i] = 0;
  }
  dir4 = InputCode::Directional::DIRECTIONAL_NONE;
}

void Input::handleKeyPressed(sf::Event::KeyEvent& key)
{
  InputCode::Key keyCode = mapKey(key);
  if (keyCode != InputCode::Key::UNKNOW) {
    keyStates.set(keyCode);
  }
}

void Input::handleKeyRelease(sf::Event::KeyEvent& key)
{
  InputCode::Key keyCode = mapKey(key);
  if (keyCode != InputCode::Key::UNKNOW) {
    keyStates.reset(keyCode);
  }
}

void Input::update()
{
  // pressStates = keyStates;
  // Log::out() << "update";
  for (int i = 1; i < InputCode::NUM_KEYS; i++) {
    previousKeyStates[i] = pressStates[i];
    pressStates[i] = keyStates[i];
    if (pressStates[i]) {
      keyTimes[i] = keyTimes[i] + 1;
      if (keyTimes[i] > 6) {
        keyTimes[i] = 0;
      }
    } else if (keyTimes[i] > 0) {
      keyTimes[i] = 0;
    }
  }

  updateDirectional();
}

bool Input::isPressed(InputCode::Key key)
{
  return pressStates[key];
}

bool Input::isTriggered(InputCode::Key key)
{
  return pressStates[key] && !previousKeyStates[key];
}

bool Input::isRepeated(InputCode::Key key)
{
  return pressStates[key] && keyTimes[key] == 1;
}

bool Input::isValidKey(int num)
{
  return num > -1 && num < InputCode::NUM_KEYS;
}

InputCode::Directional Input::getDir4()
{
  return dir4;
}

InputCode::Key Input::castKeyCode(int num)
{
  return (InputCode::Key)num;
}

/*
  ⇩⇩⇩ Private ⇩⇩⇩
*/

InputCode::Key Input::mapKey(sf::Event::KeyEvent& key)
{
  switch (key.code) {
    case sf::Keyboard::Down: {
      return InputCode::Key::DOWN;
    }
    case sf::Keyboard::Left: {
      return InputCode::Key::LEFT;
    }
    case sf::Keyboard::Right: {
      return InputCode::Key::RIGHT;
    }
    case sf::Keyboard::Up: {
      return InputCode::Key::UP;
    }
    case sf::Keyboard::Z:
      return InputCode::Key::A;
    case sf::Keyboard::Escape:
    case sf::Keyboard::X: {
      return InputCode::Key::B;
    }
    case sf::Keyboard::Enter:
    case sf::Keyboard::Space:
    case sf::Keyboard::C: {
      return InputCode::Key::C;
    }
    case sf::Keyboard::A: {
      return InputCode::Key::X;
    }
    case sf::Keyboard::S: {
      return InputCode::Key::Y;
    }
    case sf::Keyboard::D: {
      return InputCode::Key::Z;
    }
    case sf::Keyboard::Q: {
      return InputCode::Key::L;
    }
    case sf::Keyboard::W: {
      return InputCode::Key::R;
    }
    case sf::Keyboard::LShift:
    case sf::Keyboard::RShift: {
      return InputCode::Key::SHIFT;
    }
    case sf::Keyboard::LControl:
    case sf::Keyboard::RControl: {
      return InputCode::Key::CTRL;
    }
    case sf::Keyboard::LAlt:
    case sf::Keyboard::RAlt: {
      return InputCode::Key::ALT;
    }
    case sf::Keyboard::F5: {
      return InputCode::Key::F5;
    }
    case sf::Keyboard::F6: {
      return InputCode::Key::F6;
    }
    case sf::Keyboard::F7: {
      return InputCode::Key::F7;
    }
    case sf::Keyboard::F8: {
      return InputCode::Key::F8;
    }
    case sf::Keyboard::F9: {
      return InputCode::Key::F9;
    }
    default:
      return InputCode::Key::UNKNOW;
      break;
  }
}

void Input::updateDirectional()
{
  if (pressStates[InputCode::Key::UP] && dir4 != InputCode::Directional::DIRECTIONAL_DOWN) {
    dir4 = InputCode::Directional::DIRECTIONAL_UP;
    return;
  } else if (pressStates[InputCode::Key::DOWN] && dir4 != InputCode::Directional::DIRECTIONAL_UP) {
    dir4 = InputCode::Directional::DIRECTIONAL_DOWN;
    return;
  }

  if (pressStates[InputCode::Key::LEFT] && dir4 != InputCode::Directional::DIRECTIONAL_RIGHT) {
    dir4 = InputCode::Directional::DIRECTIONAL_LEFT;
    return;
  } else if (pressStates[InputCode::Key::RIGHT] && dir4 != InputCode::Directional::DIRECTIONAL_LEFT) {
    dir4 = InputCode::Directional::DIRECTIONAL_RIGHT;
    return;
  }

  dir4 = InputCode::Directional::DIRECTIONAL_NONE;
}

}  // namespace Eng