#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <bitset>

namespace Eng {

namespace InputCode {

enum Directional {
  DIRECTIONAL_NONE = 0,
  DIRECTIONAL_DOWN = 2,
  DIRECTIONAL_UP = 8,
  DIRECTIONAL_LEFT = 4,
  DIRECTIONAL_RIGHT = 6
};

enum Key {
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

}  // namespace Code

class Input {
 public:
  /*
    ⇩⇩⇩ Static ⇩⇩⇩
  */

  static void Init();
  static Input& Instance();
  static void Destroy();

  /*
    ⇩⇩⇩ Instance ⇩⇩⇩
  */

  void handleKeyPressed(sf::Event::KeyEvent& key);

  void handleKeyRelease(sf::Event::KeyEvent& key);

  void update();

  bool isPressed(InputCode::Key key);

  bool isTriggered(InputCode::Key key);

  bool isRepeated(InputCode::Key key);

  bool isValidKey(int num);

  InputCode::Directional getDir4();

  InputCode::Key castKeyCode(int num);

 private:
  std::bitset<InputCode::NUM_KEYS> keyStates;
  bool previousKeyStates[InputCode::NUM_KEYS];
  bool pressStates[InputCode::NUM_KEYS];
  unsigned short keyTimes[InputCode::NUM_KEYS];
  InputCode::Directional dir4;

  Input();

  Input(const Input&);
  Input& operator=(const Input&);

  InputCode::Key mapKey(sf::Event::KeyEvent& key);

  void updateDirectional();
};

}  // namespace Eng