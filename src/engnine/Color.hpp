#pragma once

// TODO: Add values range automatically correction

#include <SFML/Graphics/Color.hpp>
#include <stdexcept>

#include "base/MarshalUtils.hpp"
#include "engnine/EngineBase.hpp"

namespace Eng {

class Color : public EngineBase {
 public:
  static Color *deserialize(const char *data, int len)
  {
    if (len < 32) {
      throw std::runtime_error("Marshal error: Color has a bad file format");
    }

    Color *color = new Color();

    color->red = MarshalUtils::readDouble(&data);
    color->green = MarshalUtils::readDouble(&data);
    color->blue = MarshalUtils::readDouble(&data);
    color->alpha = MarshalUtils::readDouble(&data);

    color->syncSfColor();

    return color;
  }

  float red = 0;
  float green = 0;
  float blue = 0;
  float alpha = 255;

  Color()
  {
    syncSfColor();
  }

  Color(float red, float green, float blue, float alpha = 255) :
      Color()
  {
    set(red, green, blue, alpha);
  }

  void set(float r, float g, float b)
  {
    set(r, g, b, 255);
  }

  void set(float r, float g, float b, float a)
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

  void clamp(float &v)
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