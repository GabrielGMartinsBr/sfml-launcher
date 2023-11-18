#pragma once

namespace Eng {

class Rect {
 public:
  float x;
  float y;
  unsigned int width;
  unsigned int height;

  Rect(float _x, float _y, unsigned int _width, unsigned int _height)
  {
    x = _x;
    y = _y;
    width = _width;
    height = _height;
  }

  Rect(Rect *_rect)
  {
    x = _rect->x;
    y = _rect->y;
    width = _rect->width;
    height = _rect->height;
  }

  void set(int _x, int _y, int _width, int _height)
  {
    x = _x;
    y = _y;
    width = _width;
    height = _height;
  }
};

}  // namespace Eng