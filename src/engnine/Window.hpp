#pragma once

#include "engnine/Bitmap.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Viewport.hpp"

namespace Eng {

class Window {
 public:

  Window(Viewport *viewport = 0) :
      windowSkin(0),
      contents(0),
      cursor_rect(new Rect(0, 0, 16, 16))
  {
  }

  Bitmap *getWindowSkin() { return windowSkin; }
  void setWindowSkin(Bitmap *value)
  {
    windowSkin = value;
  }

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

  bool getter_stretch() { return stretch; }
  void setter_stretch(bool v) { stretch = v; }

  Rect *getter_cursor_rect() { return cursor_rect; }
  void setter_cursor_rect(Rect *v) { cursor_rect = v; }

  bool getter_active() { return active; }
  void setter_active(bool v) { active = v; }

  bool getter_visible() { return visible; }
  void setter_visible(bool v) { visible = v; }

  bool getter_pause() { return pause; }
  void setter_pause(bool v) { pause = v; }

  int getX() { return x; }
  void setX(int v) { x = v; }

  int getY() { return y; }
  void setY(int v) { y = v; }

  int getWidth() { return width; }
  void setWidth(int v) { width = v; }

  int getHeight() { return height; }
  void setHeight(int v) { height = v; }

  int getZ() { return z; }
  void setZ(int v) { z = v; }

  int getter_ox() { return ox; }
  void setter_ox(int v) { ox = v; }

  int getter_oy() { return oy; }
  void setter_oy(int v) { oy = v; }

  int getter_opacity() { return opacity; }
  void setter_opacity(int v) { opacity = v; }

  int getter_back_opacity() { return back_opacity; }
  void setter_back_opacity(int v) { back_opacity = v; }

  int getter_contents_opacity() { return contents_opacity; }
  void setter_contents_opacity(int v) { contents_opacity = v; }

 private:
  Bitmap *windowSkin;
  Bitmap *contents;
  bool stretch;
  Rect *cursor_rect;
  bool active;
  bool visible;
  bool pause;
  int x;
  int y;
  int width;
  int height;
  int z;
  int ox;
  int oy;
  int opacity;
  int back_opacity;
  int contents_opacity;
};

}  // namespace Eng