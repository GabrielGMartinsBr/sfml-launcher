#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <bitset>
#include <cstdint>

#include "AppDefs.h"

namespace ae {

using SfKeyEvent = sf::Event::KeyEvent;
using SfKey = sf::Keyboard::Key;
using app::UInt;

struct AeonInput {
  /*
    ⇩⇩⇩ Static ⇩⇩⇩
  */

  static void Init();
  static AeonInput& Instance();
  static void Destroy();

  /*
    ⇩⇩⇩ Instance ⇩⇩⇩
  */

  void handleKeyPressed(const SfKeyEvent& key);

  void handleKeyRelease(const SfKeyEvent& key);

  void update();

  bool isPressed(SfKey key);

  bool isTriggered(SfKey key);

  bool isRepeated(SfKey key);

  static inline bool isValidKey(SfKey key)
  {
    return key > SfKey::Unknown && key < SfKey::KeyCount;
  }

  static inline bool isValidKey(int8_t key)
  {
    return key > SfKey::Unknown && key < SfKey::KeyCount;
  }


 private:
  std::bitset<SfKey::KeyCount> currentKeys;
  std::bitset<SfKey::KeyCount> previousKeys;
  app::UnMap<SfKey, UInt> repeatTs;
  sf::Clock clock;

  const int repeatDelay = sf::milliseconds(500).asMilliseconds();
  const int repeatInterval = sf::milliseconds(30).asMilliseconds();

  AeonInput();

  AeonInput(const AeonInput&);
  AeonInput& operator=(const AeonInput&);
};

}  // namespace ae