#pragma once

#include "engnine/Rect.hpp"
namespace Eng {

class Viewport {
  Rect rect;

 public:
  Viewport(int x, int y, int width, int height) :
      rect(x, y, width, height) { }

  Viewport(Rect* _rect) :
      rect(_rect) { }

  Rect* getRect()
  {
    return &rect;
  }

  void setRect(Rect* _rect)
  {
    rect.x = _rect->x;
    rect.y = _rect->y;
    rect.width = _rect->width;
    rect.height = _rect->height;
  }
};

}  // namespace Eng