#pragma once

#include <SFML/Graphics/Image.hpp>

#include "engnine/Color.hpp"
#include "engnine/Rect.hpp"

namespace Eng {

typedef const char* Str;

class Bitmap {
  bool _disposed;

  sf::Image buffer;

 public:
  int width;
  int height;

  Bitmap(const char* filename);

  Bitmap(int _width, int _height)
  {
    width = _width;
    height = _height;

    buffer.create(width, height);
  }

  void dispose();

  bool disposed() { return _disposed; }

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

  void set_pixel(unsigned int _x, unsigned int _y, Color* _color)
  {
    sf::Color color(_color->red, _color->green, _color->blue);
    buffer.setPixel(_x, _y, color);
  }

  void hue_change(int _hue);

  void draw_text(int _x, int _y, int _width, int _height, Str _str, int _align);

  void draw_text(Rect _rect, Str _str, int _align);

  int get_text_size(Str _str);
};

}  // namespace Eng