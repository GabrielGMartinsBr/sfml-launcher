#pragma once

#include "engnine/Engine.hpp"
#include "engnine/Timer.hpp"

namespace Eng {

class Graphics {
  unsigned int frame_rate = 40;

  Graphics() { }

  Graphics(const Graphics&);
  Graphics& operator=(const Graphics&);

 public:
  unsigned long frame_count = 0;

  static Graphics& getInstance()
  {
    static Graphics instance;
    return instance;
  }

  unsigned int getFrameRate()
  {
    return frame_rate;
  }

  void setFrameRate(unsigned int v)
  {
    if (v < 10) {
      v = 10;
    } else if (v > 120) {
      v = 120;
    }
    frame_rate = v;
    Timer::getInstance().setFrameRate(frame_rate);
  }

  void update()
  {
    Engine::getInstance().updateGraphics();
    Engine::getInstance().updateInput();

    // Timer::getInstance().printFps();
    Timer::getInstance().controlFrameRate();

    frame_count++;
  }

  void freeze();  // TODO: Implement this method

  // TODO: Implement this method
  void transition()
  {
    Log::out() << "Graphics transition was called, but it is not implemented yet.";
  }

  // TODO: Implement this method
  void frame_reset()
  {
    Log::out() << "Graphics frame_reset was called, but it is not implemented yet.";
  }
};

}  // namespace Eng