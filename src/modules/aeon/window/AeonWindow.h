#pragma once

#include <ruby.h>

#include "engnine/Viewport.hpp"
#include "engnine/Window.h"

namespace ae {

class AeonWindow : public Eng::Window {
 public:
  AeonWindow(VALUE rbObj, Eng::Viewport *viewport = nullptr);

  ~AeonWindow();
};

}  // namespace ae