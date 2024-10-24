#include "aeon/input/AeonInput.h"

#include <SFML/Window/Keyboard.hpp>
#include <cassert>

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

AeonInput::AeonInput()
{
}

void AeonInput::handleKeyPressed(const SfKeyEvent& key)
{
  if (!currentKeys[key.code]) {
    repeatTs[key.code] = clock.getElapsedTime().asMilliseconds();
  }
  currentKeys[key.code] = true;
}

void AeonInput::handleKeyRelease(const SfKeyEvent& key)
{
  currentKeys[key.code] = false;
  repeatTs.erase(key.code);
}

void AeonInput::update()
{
  previousKeys = currentKeys;
}

bool AeonInput::isPressed(SfKey key)
{
  if (key == SfKey::Unknown) return false;
  return currentKeys[key];
}

bool AeonInput::isTriggered(SfKey key)
{
  if (key == SfKey::Unknown) return false;
  return currentKeys[key] && !previousKeys[key];
}

bool AeonInput::isRepeated(SfKey key)
{
  if (key == SfKey::Unknown || !currentKeys[key]) {
    return false;
  }
  if (!previousKeys[key]) {
    repeatTs[key] = clock.getElapsedTime().asMilliseconds();
    return true;
  }

  UInt elapsedTime = clock.getElapsedTime().asMilliseconds() - repeatTs[key];

  if (elapsedTime > repeatDelay) {
    UInt timeSinceRepeat = elapsedTime - repeatDelay;
    if (timeSinceRepeat >= repeatInterval) {
      repeatTs[key] = clock.getElapsedTime().asMilliseconds() - repeatDelay;
      return true;
    }
  }

  return false;
}

}  // namespace ae