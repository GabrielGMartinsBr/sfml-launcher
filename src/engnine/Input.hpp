#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <bitset>

namespace Eng {

enum InputKey {
  UNKNOW,
  DOWN,
  LEFT,
  RIGHT,
  UP,
  A,
  B,
  C,
  X,
  Y,
  Z,
  L,
  R,
  SHIFT,
  CTRL,
  ALT,
  F5,
  F6,
  F7,
  F8,
  F9,
  NUM_KEYS
};

class Input {
 public:

  static Input& getInstance()
  {
    static Input instance;
    return instance;
  }

  void handleKeyPressed(sf::Event::KeyEvent& key)
  {
    InputKey keyCode = mapKey(key);
    if (keyCode != InputKey::UNKNOW) {
      keyStates.set(keyCode);
    }
  }

  void handleKeyRelease(sf::Event::KeyEvent& key)
  {
    InputKey keyCode = mapKey(key);
    if (keyCode != InputKey::UNKNOW) {
      keyStates.reset(keyCode);
    }
  }

  void update()
  {
    // pressStates = keyStates;
    // Log::out() << "update";
    for (int i = 1; i < InputKey::NUM_KEYS; i++) {
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
  }

  bool isPressed(InputKey key)
  {
    return pressStates[key];
  }

  bool isTriggered(InputKey key)
  {
    return pressStates[key] && !previousKeyStates[key];
  }

  bool isRepeated(InputKey key)
  {
    return pressStates[key] && keyTimes[key] == 1;
  }

  bool isValidKey(int num)
  {
    return num > -1 && num < Eng::InputKey::NUM_KEYS;
  }

  Eng::InputKey castKeyCode(int num)
  {
    return (Eng::InputKey)num;
  }

 private:
  std::bitset<InputKey::NUM_KEYS> keyStates;
  bool previousKeyStates[InputKey::NUM_KEYS];
  bool pressStates[InputKey::NUM_KEYS];
  unsigned short keyTimes[InputKey::NUM_KEYS];

  Input()
  {
    for (int i = 1; i < InputKey::NUM_KEYS; i++) {
      keyTimes[i] = 0;
    }
  }

  Input(const Input&);
  Input& operator=(const Input&);

  InputKey mapKey(sf::Event::KeyEvent& key)
  {
    switch (key.code) {
      case sf::Keyboard::Down: {
        return InputKey::DOWN;
      }
      case sf::Keyboard::Left: {
        return InputKey::LEFT;
      }
      case sf::Keyboard::Right: {
        return InputKey::RIGHT;
      }
      case sf::Keyboard::Up: {
        return InputKey::UP;
      }
      case sf::Keyboard::Z:
        return InputKey::A;
      case sf::Keyboard::Escape:
      case sf::Keyboard::X: {
        return InputKey::B;
      }
      case sf::Keyboard::Enter:
      case sf::Keyboard::Space:
      case sf::Keyboard::C: {
        return InputKey::C;
      }
      case sf::Keyboard::A: {
        return InputKey::X;
      }
      case sf::Keyboard::S: {
        return InputKey::Y;
      }
      case sf::Keyboard::D: {
        return InputKey::Z;
      }
      case sf::Keyboard::Q: {
        return InputKey::L;
      }
      case sf::Keyboard::W: {
        return InputKey::R;
      }
      case sf::Keyboard::LShift:
      case sf::Keyboard::RShift: {
        return InputKey::SHIFT;
      }
      case sf::Keyboard::LControl:
      case sf::Keyboard::RControl: {
        return InputKey::CTRL;
      }
      case sf::Keyboard::LAlt:
      case sf::Keyboard::RAlt: {
        return InputKey::ALT;
      }
      case sf::Keyboard::F5: {
        return InputKey::F5;
      }
      case sf::Keyboard::F6: {
        return InputKey::F6;
      }
      case sf::Keyboard::F7: {
        return InputKey::F7;
      }
      case sf::Keyboard::F8: {
        return InputKey::F8;
      }
      case sf::Keyboard::F9: {
        return InputKey::F9;
      }
      default:
        return InputKey::UNKNOW;
        break;
    }
  }
};

}  // namespace Eng