#pragma once

#include "aeon/window/AeonWindow.h"

namespace ae {

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
  void update();
};

}  // namespace ae