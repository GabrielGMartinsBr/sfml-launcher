#pragma once

// TODO: Add values range automatically correction

#include <SFML/Graphics/Color.hpp>
#include <stdexcept>

#include "base/MarshalUtils.hpp"
#include "ruby.h"

namespace Eng {

class Color {
 public:
  VALUE ptr;

  static Color *deserialize(const char *data, int len)
  {
    if (len < 32) {
      throw std::runtime_error("Marshal error: Color has a bad file format");
    }

    Color *color = new Color();

    color->red = MarshalUtils::readDouble(&data);
    color->green = MarshalUtils::readDouble(&data);
    color->blue = MarshalUtils::readDouble(&data);

    color->syncSfColor();

    return color;
  }

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

  sf::Color &getSfColor()
  {
    return sfColor;
  }

  void sync()
  {
    syncSfColor();
  }

 private:
  sf::Color sfColor;

  void clamp(unsigned int &v)
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