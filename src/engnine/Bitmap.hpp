#pragma once

#include "engnine/Color.hpp"
#include "engnine/Rect.hpp"

namespace Eng {

typedef const char* Str;

class Bitmap {
  int width;
  int height;
  bool _disposed;

 public:
  Bitmap(const char* filename);

  Bitmap(int _width, int _height)
  {
    width = _width;
    height = _height;
  }

  void dispose();

  bool disposed() { return _disposed; }

  int get_width() { return width; }

  int get_height() { return height; }

  void get_rect(){};

  void blt(){};

  void stretch_blt(){};

  void fill_rect(int _x, int _y, int _width, int _height, Color _color){};

  void fill_rect(int _rect, Color _color){};

  void clear();

  void get_pixel(int _x, int _y);

  void set_pixel(int _x, int _y, Color _color);

  void hue_change(int _hue);

  void draw_text(int _x, int _y, int _width, int _height, Str _str, int _align);

  void draw_text(Rect _rect, Str _str, int _align);

  int get_text_size(Str _str);
};

}  // namespace Eng