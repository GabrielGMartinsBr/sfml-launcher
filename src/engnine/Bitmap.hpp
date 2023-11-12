#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <stdexcept>

#include "engnine/Color.hpp"
#include "engnine/Rect.hpp"

namespace Eng {

typedef const char* Str;

class Bitmap {
 public:
  bool _disposed = false;
  sf::Image buffer;

  unsigned int width;
  unsigned int height;

  bool _needsUpdate = false;

  Bitmap(const char* filename);

  Bitmap(unsigned int _width, unsigned int _height) :
      buffer()
  {
    width = _width;
    height = _height;

    buffer.create(width, height, sf::Color::Transparent);
  }

  void dispose()
  {
    _disposed = true;
  }

  bool disposed()
  {
    return _disposed;
  }

  void get_rect(){};

  void blt(){};

  void stretch_blt(){};

  void fill_rect(int _x, int _y, int _width, int _height, Color _color){};

  void fill_rect(int _rect, Color _color){};

  void clear();

  Color* get_pixel(unsigned int _x, unsigned int _y)
  {
    sf::Color px = buffer.getPixel(_x, _y);
    Color* color = new Color(px.r, px.g, px.b);
    return color;
  }

  void set_pixel(unsigned int x, unsigned int y, Color* _color)
  {
    if (_disposed) {
      throw std::runtime_error("disposed bitmap");
    }
    if (x < 0 || x > width || y < 0 || y > height) {
      return;
    }
    sf::Color color(_color->red, _color->green, _color->blue);
    buffer.setPixel(x, y, color);
    _needsUpdate = true;
  }

  void hue_change(int _hue);

  void draw_text(int _x, int _y, int _width, int _height, Str _str, int _align);

  void draw_text(Rect _rect, Str _str, int _align);

  int get_text_size(Str _str);
};

}  // namespace Eng