#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "engnine/Bitmap.hpp"
#include "engnine/Color.hpp"
#include "engnine/RGSSViewport.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Tone.hpp"
#include "engnine/Viewport.hpp"
#include "ruby.h"

namespace Eng {

class Sprite {
 public:
  sf::Sprite sprite;
  sf::Texture text;

  VALUE bitmap_ptr;  // TODO: Remove

  int zoom_x;
  int zoom_y;
  int angle;
  int mirror;
  int bush_depth;
  int blend_type;

  Sprite() { }

  Sprite(Viewport *_viewport)
  {
    viewport = _viewport;
  }

  /*
    Attr x
  */
  int getX() { return x; }
  void setX(int _x)
  {
    x = _x;
    dirty = true;
  }

  /*
    Attr y
  */
  int getY() { return y; }
  void setY(int _y)
  {
    y = _y;
    dirty = true;
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
    dirty = true;
  }

  /*
    Attr oy
  */
  int getOy() { return oy; }
  void setOy(int _oy)
  {
    oy = _oy;
    dirty = true;
  }

  /*
    Attr bitmap
  */

  void setBitmap(Bitmap *_bitmap)
  {
    bitmap = _bitmap;
    dirty = true;
  }

  Bitmap *getBitmap()
  {
    return bitmap;
  }

  /*
   Attr src_rect
  */

  void setSrcRect(Rect *_src_rect)
  {
    src_rect = _src_rect;
    dirty = true;
  }

  Rect *getSrcRect()
  {
    return src_rect;
  }

  void setSrcRect(int x, int y, int w, int h)
  {
    sprite.setTextureRect(
      sf::IntRect(x, y, w, h)
    );
  }

  void dispose()
  {
    _disposed = true;
  }

  bool disposed()
  {
    return _disposed;
  }

  Viewport *getViewport()
  {
    return viewport;
  }

  void update() { }

  bool shouldRender()
  {
    return !_disposed && bitmap != nullptr && !bitmap->disposed();
  }

  void applyChanges()
  {
    if (!bitmap) {
      return;
    }

    if (bitmap->dirty) {
      text.loadFromImage(bitmap->buffer);
      sprite.setTexture(text);
      bitmap->dirty = false;
    }

    if (!dirty) {
      return;
    }

    if (src_rect) {
      setSrcRect(
        src_rect->x,
        src_rect->y,
        src_rect->width,
        src_rect->height
      );
    }

    sprite.setPosition(x - ox, y - oy);

    dirty = false;
  }

  void renderInViewport(RGSS::Viewport &defaultViewport)
  {
    if (!shouldRender()) {
      return;
    }
    applyChanges();
    if (viewport) {
      viewport->getRgssViewport().draw(
        sprite
      );
    } else {
      defaultViewport.draw(sprite);
    }
  }

 private:
  bool dirty = false;
  bool _disposed = false;

  Viewport *viewport = nullptr;
  Bitmap *bitmap = nullptr;
  Color *color = nullptr;
  Tone *tone = nullptr;
  Rect *src_rect = nullptr;

  float x = 0;
  float y = 0;
  int z = 0;
  bool visible = true;
  float ox = 0;
  float oy = 0;
  int opacity = 255;
};

}  // namespace Eng