#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "aeon/input/AeonInput.h"

namespace Eng {

using ae::AeonInput;
using SfKeyEvent = sf::Event::KeyEvent;
using SfKey = sf::Keyboard::Key;
using app::UInt;

class Input {
 public:
  /*
    ⇩⇩⇩ Static ⇩⇩⇩
  */

  static void Init();
  static Input& Instance();
  static void Destroy();

  enum DirectionalCode {
    DIRECTIONAL_NONE = 0,
    DIRECTIONAL_DOWN = 2,
    DIRECTIONAL_UP = 8,
    DIRECTIONAL_LEFT = 4,
    DIRECTIONAL_RIGHT = 6
  };

  enum KeyCode {
    UNKNOW = -1,
    DOWN = 0,
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
    KEY_COUNT
  };

  /*
    ⇩⇩⇩ Instance ⇩⇩⇩
  */
  void handleKeyPressed(const SfKeyEvent& key);
  void handleKeyRelease(const SfKeyEvent& key);

  void update();

  bool isPressed(KeyCode key);

  bool isTriggered(KeyCode key);

  bool isRepeated(KeyCode key);

  bool isValidKey(int num);

  DirectionalCode getDir4();

 private:
  EngineClock& clock;
  bool currentKeys[KEY_COUNT];
  bool previousKeys[KEY_COUNT];
  bool pressKeys[KEY_COUNT];
  UInt repeatTs[KEY_COUNT];
  DirectionalCode dir4;

  const int repeatDelay = sf::milliseconds(500).asMilliseconds();
  const int repeatInterval = sf::milliseconds(30).asMilliseconds();

  Input();

  Input(const Input&);
  Input& operator=(const Input&);

  void updateDirectional();

  KeyCode mapKey(const SfKey& key) const;
};

}  // namespace Eng