#pragma once

#include <chrono>
#include <cmath>
#include <thread>

#include "base/Log.hpp"

namespace Eng {

typedef std::chrono::high_resolution_clock::time_point T_TimePoint;
typedef std::chrono::milliseconds T_Milliseconds;

class Timer {
  T_TimePoint lastFrameTs;
  float frameTime = 1.0f / 40.0f * 1e3;
  float fps = 0;

  Timer() = default;
  Timer(const Timer&) = delete;
  Timer& operator=(const Timer&) = delete;

 public:
  static Timer& getInstance()
  {
    static Timer instance;
    return instance;
  }

  void setFrameRate(float frameRate)
  {
    frameTime = 1.0f / frameRate * 1e3;
  }

  void updateLastFrame()
  {
    lastFrameTs = Timer::getNow();
  }

  float getElapsed() const
  {
    T_TimePoint currentTs = getNow();
    auto duration = std::chrono::duration_cast<T_Milliseconds>(currentTs - lastFrameTs);
    return duration.count();
  }

  void printFps()
  {
    Log::out() << "FPS: " << std::round(fps);
  }

  void controlFrameRate()
  {
    float elapsed = getElapsed();
    if (elapsed < frameTime) {
      float d = frameTime - elapsed;
      std::this_thread::sleep_for(
        std::chrono::milliseconds((unsigned int)d)
      );
      elapsed += d;
    }
    fps = 1.0e3f / elapsed;
    updateLastFrame();
  }

  //   Utils

  static T_TimePoint getNow()
  {
    return std::chrono::high_resolution_clock::now();
  }
};

}  // namespace Eng