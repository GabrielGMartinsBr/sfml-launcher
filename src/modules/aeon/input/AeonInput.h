#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cstdint>

#include "AppDefs.h"
#include "aeon/input/AeonInputKeys.h"
#include "engine/EngineClock.h"

namespace ae {

using SfKeyEvent = sf::Event::KeyEvent;
using SfMouseButtonEvent = sf::Event::MouseButtonEvent;
using SfMouseMoveEvent = sf::Event::MouseMoveEvent;

using app::UInt;
using Eng::EngineClock;
using sf::Vector2i;

struct AeonInput {
  /*
    ⇩⇩⇩ Static ⇩⇩⇩
  */

  static void Init();
  static AeonInput& Instance();
  static void Destroy();

  static inline bool isValidInputCode(int8_t key)
  {
    return key > InputCode::Unknown && key < InputCode::Count;
  }

  /*
    ⇩⇩⇩ Instance ⇩⇩⇩
  */
  static inline bool isValidMouseButton(int8_t key)
  {
    return key > InputCode::KeyCount && key < InputCode::MouseButtonCount;
  }

  void handleKeyPressed(const SfKeyEvent& key);
  void handleKeyRelease(const SfKeyEvent& key);

  void handleMousePressed(const SfMouseButtonEvent& button);
  void handleMouseRelease(const SfMouseButtonEvent& button);

  void handleMouseMoved(const SfMouseMoveEvent& mouse);

  void update();

  bool isPressed(int8_t key);

  bool isTriggered(int8_t key);

  bool isRepeated(int8_t key);

  inline const Vector2i& getMousePosition() const
  {
    return mousePosition;
  }


 private:
  EngineClock& clock;
  bool currentKeys[InputCode::Count];
  bool previousKeys[InputCode::Count];
  UInt repeatTs[InputCode::Count];
  Vector2i mousePosition;

  const int repeatDelay = sf::milliseconds(500).asMilliseconds();
  const int repeatInterval = sf::milliseconds(30).asMilliseconds();

  AeonInput();

  AeonInput(const AeonInput&);
  AeonInput& operator=(const AeonInput&);
};

}  // namespace ae