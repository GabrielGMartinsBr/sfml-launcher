#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "AppDefs.h"
#include "aeon/base/AeonEvents.h"
#include "aeon/window/AeonWindow.h"

namespace ae {

using app::ULong;
using app::Vector;

struct AeonWindowManager {
  /*
    ⇩⇩⇩ Static ⇩⇩⇩
  */

  static void Init();
  static AeonWindowManager& Instance();
  static void Destroy();

  /*
    ⇩⇩⇩ Instance ⇩⇩⇩
  */

  AeonWindowManager();

  Vector<AeonWindow*> entries;
  AeonWindow* focusedWindow;

  void handleMouseMoved(const AeMouseMoveEvent& event);
  void handleMousePressed(const AeMouseButtonEvent& event);
  void handleMouseReleased(const AeMouseButtonEvent& event);
  void handleKeyPressed(const AeKeyEvent& event);
  void handleTextEntered(const AeTextEvent& event);

  void addEntry(AeonWindow* entry);
  void removeEntry(AeonWindow* entry);
  void updateEntries();

  ULong getTimestamp();

  void setFocusOn(AeonWindow* window);

 private:
  sf::Clock clock;
  ULong ts;
};

}  // namespace ae