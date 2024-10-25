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
  std::memset(currentKeys, 0, SfKey::KeyCount);
  std::memset(previousKeys, 0, SfKey::KeyCount);
  std::memset(repeatTs, 0, SfKey::KeyCount);
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

void AeonInput::update()
{
  std::memcpy(previousKeys, currentKeys, SfKey::KeyCount);
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

  const UInt timestamp = clock.getElapsedTime().asMilliseconds();

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