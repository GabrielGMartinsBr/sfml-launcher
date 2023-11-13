#pragma once

#include "engnine/Engine.hpp"
namespace Eng {

class Graphics {

 public:
  static void update()
  {
    Engine::getInstance().updateGraphics();
  }
};

}  // namespace Eng