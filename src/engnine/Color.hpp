#pragma once

// TODO: Add values range automatically correction

namespace Eng {

class Color {
 public:
  int red = 0;
  int green = 0;
  int blue = 0;

  Color(int r, int g, int b)
  {
    set(r, g, b);
  }

  void set(int r, int g, int b)
  {
    clamp(r);
    clamp(g);
    clamp(b);
    red = r;
    green = g;
    blue = b;
  }

 private:
  void clamp(int& v)
  {
    if (v < 0) {
      v = 0;
    } else if (v > 255) {
      v = 255;
    }
  }
};

}  // namespace Eng