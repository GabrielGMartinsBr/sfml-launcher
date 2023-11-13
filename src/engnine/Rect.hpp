#pragma once

namespace Eng {

class Rect {
 public:
  int x;
  int y;
  int width;
  int height;

  Rect(int _x, int _y, int _width, int _height)
  {
    x = _x;
    y = _y;
    width = _width;
    height = _height;
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