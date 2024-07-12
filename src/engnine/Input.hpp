#pragma once

#include "engnine/Engine.hpp"

namespace Eng {

enum InputKey {
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
  F9
};

class Input {

 public:

  void update()
  {
    Engine::getInstance().updateInput();
  }
};

}  // namespace Eng