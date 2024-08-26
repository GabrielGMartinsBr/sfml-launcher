#pragma once

#include "engnine/IOnRender.h"
#include "engnine/Viewport.hpp"

namespace Eng {

struct IViewportChild : public IOnRender {
  virtual Viewport* getViewport() const = 0;
};

}  // namespace Eng