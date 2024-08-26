#pragma once

#include "engnine/OnRender.h"
#include "engnine/Viewport.hpp"

namespace Eng {

struct IViewportChild : public OnRender {
  virtual Viewport* getViewport() const = 0;
};

}  // namespace Eng