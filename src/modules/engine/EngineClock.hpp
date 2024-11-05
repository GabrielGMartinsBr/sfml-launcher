#pragma once

#include <SFML/System/Clock.hpp>

namespace Eng {

struct EngineClock {
  using Clock = sf::Clock;

  // Constructor
  EngineClock(float frameRate = 40.0f);

  void restart();

  // Public methods for updating the engine
  void update();
  void updateBatterySaving();
  void updateHighResponsiveness();
  void updateEngineComponents(float deltaTime);

  // Public getters
  inline const Clock& getClock() const { return clock; }
  inline int getTotalElapsedTime() const { return clock.getElapsedTime().asMilliseconds(); }
  inline float getFps() const { return fps; }
  inline bool isBatterySavingMode() const { return batterySavingMode; }

  // Public setter
  inline void setBatterySavingMode(bool value) { batterySavingMode = value; }
  void setFrameRate(float value);

 private:
  bool batterySavingMode;
  Clock clock;
  Clock frameClock;
  float frameRate;
  float fixedDeltaTime;
  float targetFrameTime;
  float accumulatedTs;
  float fps;
};

}  // namespace Eng
