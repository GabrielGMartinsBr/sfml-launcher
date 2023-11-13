#pragma once

#include "engnine/Engine.hpp"

namespace Eng {

class Graphics {
  Graphics() { }

  Graphics(const Graphics&);
  Graphics& operator=(const Graphics&);

 public:
  unsigned int frame_rate = 40;
  unsigned long frame_count = 0;

  static Graphics& getInstance()
  {
    static Graphics instance;
    return instance;
  }

  void update()
  {
    Engine::getInstance().updateGraphics();
    frame_count++;
  }

  void freeze();  // TODO: Implement this method

  void transition();  // TODO: Implement this method

  void frame_reset();  // TODO: Implement this method
};

}  // namespace Eng