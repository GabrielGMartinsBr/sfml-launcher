#pragma once

#include <SFML/Graphics/Rect.hpp>

#include "engnine/RGSSViewport.hpp"
#include "engnine/Rect.hpp"
#include "ruby.h"

namespace Eng {

class Viewport {
 public:
  VALUE ptr;

  Viewport(int x, int y, int width, int height) :
      rect(x, y, width, height),
      rgssVp(x, y, width, height)
  {
  }

  Viewport(Rect* _rect) :
      rect(_rect),
      rgssVp(rect)
  {
  }

  RGSS::Viewport& getRgssViewport()
  {
    return rgssVp;
  }

  /*
    Attr rect
  */
  Rect* getRect() { return &rect; }
  void setRect(Rect* _rect)
  {
    rect.x = _rect->x;
    rect.y = _rect->y;
    rect.width = _rect->width;
    rect.height = _rect->height;
  }

  /*
    Attr visible
  */
  int getVisible() { return visible; }
  void setVisible(int _visible)
  {
    visible = _visible;
  }

  /*
    Attr z
  */
  int getZ() { return z; }
  void setZ(int _z)
  {
    z = _z;
  }

  /*
    Attr ox
  */
  int getOx() { return ox; }
  void setOx(int _ox)
  {
    ox = _ox;
  }

  /*
    Attr oy
  */
  int getOy() { return oy; }
  void setOy(int _oy)
  {
    oy = _oy;
  }

 private:
  Rect rect;
  bool visible = true;
  int z = 0;
  int ox = 0;
  int oy = 0;

  RGSS::Viewport rgssVp;
};

}  // namespace Eng