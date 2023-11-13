#pragma once

#include "engnine/Engine.hpp"

namespace Eng {

class Graphics {
  Graphics() { }

  Graphics(const Graphics&);
  Graphics& operator=(const Graphics&);

 public:
  static Graphics& getInstance()
  {
    static Graphics instance;
    return instance;
  }

  unsigned int frame_rate = 40;
  unsigned long frame_count = 0;

  void update()
  {
    Engine::getInstance().updateGraphics();
    frame_count++;
  }
};

}  // namespace Eng