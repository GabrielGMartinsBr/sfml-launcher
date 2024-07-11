#pragma once

#include "engnine/Bitmap.hpp"
#include "engnine/Viewport.hpp"

namespace Eng {

class Window {
 public:

  Window(Viewport *viewport = 0) :
      windowSkin(0),
      contents(0)
  {
  }

  Bitmap *getWindowSkin() { return windowSkin; }
  void setWindowSkin(Bitmap *value)
  {
    windowSkin = value;
  }

  int getX() { return x; }
  void setX(int v) { x = v; }

  int getY() { return y; }
  void setY(int v) { y = v; }

  int getZ() { return z; }
  void setZ(int v) { z = v; }

  int getWidth() { return width; }
  void setWidth(int v) { width = v; }

  int getHeight() { return height; }
  void setHeight(int v) { height = v; }

  Bitmap *getContents()
  {
    return contents;
  }
  void setContents(Bitmap *v)
  {
    if (contents == v) {
      return;
    }

    contents = v;
  }

 private:
  Bitmap *windowSkin;
  Bitmap *contents;
  int x;
  int y;
  int z;
  int width;
  int height;
};

}  // namespace Eng