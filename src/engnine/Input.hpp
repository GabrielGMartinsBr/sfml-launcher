#pragma once

#include "engnine/Engine.hpp"

namespace Eng {

class Input {

 public:

  void update()
  {
    Engine::getInstance().updateInput();
  }
};

}  // namespace Eng