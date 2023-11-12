#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "base/Log.hpp"
#include "engnine/Bitmap.hpp"
#include "engnine/Color.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Tone.hpp"
#include "ruby.h"

namespace Eng {

class Sprite {
  Bitmap *bitmap = nullptr;
  bool _disposed = false;

 public:
  sf::Sprite sprite;
  sf::Texture text;

  VALUE bitmap_ptr;
  Rect src_rect;
  bool visible;
  int x;
  int y;
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
  Color color;
  Tone tone;

  Sprite() :
      color(0, 0, 0),
      tone(0, 0, 0)
  {
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
    if (bitmap && bitmap->_needsUpdate) {
      text.update(bitmap->buffer);
      bitmap->_needsUpdate = false;
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

  void viewport() { }

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