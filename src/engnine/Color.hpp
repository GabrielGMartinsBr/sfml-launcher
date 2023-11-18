#pragma once

// TODO: Add values range automatically correction

#include <SFML/Graphics/Color.hpp>
namespace Eng {

class Color {
 public:
  unsigned int red = 0;
  unsigned int green = 0;
  unsigned int blue = 0;
  unsigned int alpha = 255;

  Color()
  {
    syncSfColor();
  }

  Color(unsigned int r, unsigned int g, unsigned int b) :
      Color(r, g, b, 255)
  {
  }

  Color(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
  {
    set(r, g, b, a);
  }

  void set(unsigned int r, unsigned int g, unsigned int b)
  {
    set(r, g, b, 255);
  }

  void set(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
  {
    clamp(r);
    clamp(g);
    clamp(b);
    clamp(a);
    red = r;
    green = g;
    blue = b;
    alpha = a;
    syncSfColor();
  }

  sf::Color& getSfColor()
  {
    return sfColor;
  }

  void sync()
  {
    syncSfColor();
  }

 private:
  sf::Color sfColor;

  void clamp(unsigned int& v)
  {
    if (v < 0) {
      v = 0;
    } else if (v > 255) {
      v = 255;
    }
  }

  void syncSfColor()
  {
    sfColor.r = red;
    sfColor.b = blue;
    sfColor.g = green;
    sfColor.a = alpha;
  }
};

}  // namespace Eng