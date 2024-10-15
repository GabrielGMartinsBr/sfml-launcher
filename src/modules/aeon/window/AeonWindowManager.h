#pragma once

#include <SFML/System/Clock.hpp>

#include "AppDefs.h"
#include "aeon/window/AeonWindow.h"

namespace ae {

using app::UInt;
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

  Vector<AeonWindow*> entries;

  void addEntry(AeonWindow* entry);
  void removeEntry(AeonWindow* entry);
  void updateEntries();

  UInt getTimestamp();

 private:
  sf::Clock clock;
  UInt ts;
};

}  // namespace ae