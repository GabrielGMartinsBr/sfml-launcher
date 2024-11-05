#include "aeon/input/AeonInput.h"

#include <SFML/Window/Keyboard.hpp>
#include <cassert>

#include "aeon/input/AeonInputKeys.h"

namespace ae {

/*
  ⇩⇩⇩ Static ⇩⇩⇩
*/
static AeonInput* instance = nullptr;

void AeonInput::Init()
{
  assert(!instance);
  instance = new AeonInput();
}

AeonInput& AeonInput::Instance()
{
  assert(instance);
  return *instance;
}

void AeonInput::Destroy()
{
  assert(instance);
  delete instance;
  instance = nullptr;
}

/*
  ⇩⇩⇩ Instance ⇩⇩⇩
*/

AeonInput::AeonInput() :
    clock(EngineClock::Instance()),
    mousePosition(0, 0)
{
  std::memset(currentKeys, 0, InputCode::Count);
  std::memset(previousKeys, 0, InputCode::Count);
  std::memset(repeatTs, 0, InputCode::Count);
}

void AeonInput::handleKeyPressed(const SfKeyEvent& key)
{
  if (key.code == SfKey::Unknown) return;
  currentKeys[key.code] = true;
}

void AeonInput::handleKeyRelease(const SfKeyEvent& key)
{
  currentKeys[key.code] = false;
}

void AeonInput::handleMousePressed(const SfMouseButtonEvent& button)
{
  currentKeys[button.button + InputCode::KeyCount] = true;
}

void AeonInput::handleMouseRelease(const SfMouseButtonEvent& button)
{
  currentKeys[button.button + InputCode::KeyCount] = false;
}

void AeonInput::handleMouseMoved(const SfMouseMoveEvent& mouse)
{
  mousePosition.x = mouse.x;
  mousePosition.y = mouse.y;
}

void AeonInput::update()
{
  std::memcpy(previousKeys, currentKeys, InputCode::Count);
}

bool AeonInput::isPressed(int8_t key)
{
  if (key == InputCode::Unknown) return false;
  return currentKeys[key];
}

bool AeonInput::isTriggered(int8_t key)
{
  if (key == InputCode::Unknown) return false;
  return currentKeys[key] && !previousKeys[key];
}

bool AeonInput::isRepeated(int8_t key)
{
  if (key == SfKey::Unknown || !currentKeys[key]) {
    return false;
  }

  const UInt timestamp = clock.getTotalElapsedTime();

  if (!previousKeys[key]) {
    repeatTs[key] = timestamp;
    return true;
  }

  UInt elapsedTime = timestamp - repeatTs[key];

  if (elapsedTime > repeatDelay) {
    UInt timeSinceRepeat = elapsedTime - repeatDelay;
    if (timeSinceRepeat >= repeatInterval) {
      repeatTs[key] = timestamp - repeatDelay;
      return true;
    }
  }

  return false;
}

}  // namespace ae