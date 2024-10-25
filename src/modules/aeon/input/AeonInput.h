#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cstdint>

#include "AppDefs.h"
#include "aeon/input/AeonInputKeys.h"

namespace ae {

using SfKeyEvent = sf::Event::KeyEvent;
using SfMouseButtonEvent = sf::Event::MouseButtonEvent;
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

  void handleMousePressed(const SfMouseButtonEvent& key);
  
  void handleMouseRelease(const SfMouseButtonEvent& key);

  void update();

  bool isPressed(int8_t key);

  bool isTriggered(int8_t key);

  bool isRepeated(int8_t key);

  inline const sf::Clock& getClock()
  {
    return clock;
  }

  static inline bool isValidInputCode(int8_t key)
  {
    return key > InputCode::Unknown && key < InputCode::Count;
  }

  static inline bool isValidKey(int8_t key)
  {
    return key > InputCode::Unknown && key < InputCode::KeyCount;
  }

  static inline bool isValidMouseButton(int8_t key)
  {
    return key > InputCode::KeyCount && key < InputCode::MouseButtonCount;
  }

 private:
  bool currentKeys[InputCode::Count];
  bool previousKeys[InputCode::Count];
  UInt repeatTs[InputCode::Count];
  sf::Clock clock;

  const int repeatDelay = sf::milliseconds(500).asMilliseconds();
  const int repeatInterval = sf::milliseconds(30).asMilliseconds();

  AeonInput();

  AeonInput(const AeonInput&);
  AeonInput& operator=(const AeonInput&);
};

}  // namespace ae