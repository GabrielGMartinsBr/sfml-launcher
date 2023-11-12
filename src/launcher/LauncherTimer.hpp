#pragma once

#include <chrono>
#include <cmath>
#include <thread>

#include "base/Log.hpp"

typedef std::chrono::high_resolution_clock::time_point TimePointT;

class LauncherTimer {
  TimePointT currentTime;
  TimePointT previousTime;

  float frameTime = 1.0f / 60.0f * 1e6;
  float elapsed = 0;
  float fps = 0;


 public:
  void update()
  {
    currentTime = std::chrono::high_resolution_clock::now();
    elapsed = (float)std::chrono::duration_cast<std::chrono::microseconds>(currentTime - previousTime).count();
  }

  void sleep()
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(9));
  }

  bool shouldUpdate()
  {
    return elapsed >= frameTime;
  }

  void markUpdate()
  {
    previousTime = currentTime;
  }

  void printFps()
  {
    fps = (float)1e6 / elapsed;
    Log::out() << "FPS: " << std::floor(fps);
  }
};