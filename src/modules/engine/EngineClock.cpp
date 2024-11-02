#include "engine/EngineClock.hpp"

#include <chrono>
#include <thread>

namespace Eng {

namespace ThisThread = std::this_thread;
namespace Chrono = std::chrono;

/*
 * Initializes EngineClock with specified frame rate.
 */
EngineClock::EngineClock(float frameRate) :
    batterySavingMode(true),
    frameRate(frameRate),
    fixedDeltaTime(1.0f / frameRate),
    targetFrameTime(1000.0f / frameRate),
    accumulatedTs(0.0f),
    fps(0.0f)
{
  clock.restart();
  frameClock.restart();
}

/*
 * Main update function: chooses between power-saving and responsiveness modes.
 */
void EngineClock::update()
{
  batterySavingMode ? updateBatterySaving() : updateHighResponsiveness();
}

/*
 * Battery-saving update: renders once per frame and caps frame rate with sleep.
 */
void EngineClock::updateBatterySaving()
{
  float elapsed = frameClock.restart().asMilliseconds();

  // Enforce target frame rate
  if (elapsed < targetFrameTime) {
    float sleepTime = targetFrameTime - elapsed;
    ThisThread::sleep_for(Chrono::milliseconds(static_cast<int>(sleepTime)));
    elapsed = targetFrameTime;
  }

  float fps = 1000.0f / elapsed;

  // Update engine components
  updateEngineComponents(fixedDeltaTime);

  // Ruby execution will return immediately after this method completes
}

/*
 * High-responsiveness update: uses fixed timestep and allows faster frame rates.
 */
void EngineClock::updateHighResponsiveness()
{
  // Measure time since the last frame
  float frameDuration = frameClock.restart().asSeconds();
  accumulatedTs += frameDuration;

  // Calculate sleep time to maintain target frame rate
  float sleepTime = (1.0f / (targetFrameTime / 1000.0f)) - frameDuration;
  if (sleepTime > 0) {
    ThisThread::sleep_for(Chrono::milliseconds(static_cast<int>(sleepTime * 1000)));
  }

  // Update engine components at fixed intervals
  while (accumulatedTs >= fixedDeltaTime) {
    updateEngineComponents(fixedDeltaTime);
    accumulatedTs -= fixedDeltaTime;
  }

  // Ruby execution will return immediately after this method completes
}

/*
 * Updates the engine components
 */
void EngineClock::updateEngineComponents(float deltaTime) { }

/*
 * Update frame rate values
 */
void EngineClock::setFrameRate(float value)
{
  if (value > 0.0f) {
    fixedDeltaTime = 1.0f / value;
    targetFrameTime = 1000.0f / value;
  }
}

}  // namespace Eng
