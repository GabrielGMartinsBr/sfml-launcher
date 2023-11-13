#pragma once

// TODO: Add values range automatically correction

namespace Eng {

class Color {
 public:
  unsigned int red = 0;
  unsigned int green = 0;
  unsigned int blue = 0;

  Color(unsigned int r, unsigned int g, unsigned int b)
  {
    set(r, g, b);
  }

  void set(unsigned int r, unsigned int g, unsigned int b)
  {
    clamp(r);
    clamp(g);
    clamp(b);
    red = r;
    green = g;
    blue = b;
  }

 private:
  void clamp(unsigned int& v)
  {
    if (v < 0) {
      v = 0;
    } else if (v > 255) {
      v = 255;
    }
  }
};

}  // namespace Eng