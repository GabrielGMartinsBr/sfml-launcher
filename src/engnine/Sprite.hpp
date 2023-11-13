#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "engnine/Bitmap.hpp"
#include "engnine/Color.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Tone.hpp"
#include "engnine/Viewport.hpp"
#include "ruby.h"

namespace Eng {

class Sprite {
  bool dirty = false;
  bool _disposed = false;

  Viewport *viewport = nullptr;
  Bitmap *bitmap = nullptr;
  Color *color = nullptr;
  Tone *tone = nullptr;

  int x = 0;
  int y = 0;

 public:
  sf::Sprite sprite;
  sf::Texture text;

  VALUE bitmap_ptr;
  Rect *src_rect = nullptr;
  bool visible;
  int z;
  int ox;
  int oy;
  int zoom_x;
  int zoom_y;
  int angle;
  int mirror;
  int bush_depth;
  int opacity;
  int blend_type;

  Sprite() { }

  Sprite(Viewport *_viewport)
  {
    viewport = _viewport;
  }

  int getX() { return x; }
  void setX(int _x)
  {
    x = _x;
    dirty = true;
  }

  int getY() { return y; }
  void setY(int _y)
  {
    y = _y;
    dirty = true;
  }

  void setBitmap(Bitmap *_bitmap)
  {
    bitmap = _bitmap;
    if (bitmap) {
      text.loadFromImage(bitmap->buffer);
      sprite.setTexture(text);
    }
  }

  Bitmap *getBitmap()
  {
    return bitmap;
  }

  void atualizar()
  {
    if (dirty) {
      sprite.setPosition(x, y);
      dirty = false;
    }
    if (bitmap && bitmap->dirty) {
      text.update(bitmap->buffer);
      bitmap->dirty = false;
    }
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

  void update()
  {
    atualizar();
  }

  bool shouldRender()
  {
    return !_disposed && bitmap != nullptr && !bitmap->disposed();
  }
};

}  // namespace Eng