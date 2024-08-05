#pragma once

// TODO: Add values range automatically correction

#include <SFML/Graphics/Color.hpp>
#include <stdexcept>

#include "base/MarshalUtils.hpp"
#include "engnine/EngineBase.hpp"

namespace Eng {

class Color : public EngineBase {
 public:
  static constexpr int SERIAL_SIZE = 32;

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

  void serialize(char *buffer) const
  {
    MarshalUtils::writeDouble(&buffer, red);
    MarshalUtils::writeDouble(&buffer, green);
    MarshalUtils::writeDouble(&buffer, blue);
    MarshalUtils::writeDouble(&buffer, alpha);
  }

  double red;
  double green;
  double blue;
  double alpha;

  Color() :
      Color(0, 0, 0)
  {
  }

  Color(double red, double green, double blue, double alpha = 255)
  {
    set(red, green, blue, alpha);
    syncSfColor();
  }

  ~Color()
  {
  }

  bool operator==(const Color &other) const
  {
    return red == other.red && blue == other.blue && green == other.green && alpha == other.alpha;
  }

  Color &operator=(const Color &other)
  {
    red = other.red;
    green = other.green;
    blue = other.blue;
    alpha = other.alpha;
    syncSfColor();
    return *this;
  }

  void set(double r, double g, double b, double a = 255)
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

  void clamp(double &v)
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