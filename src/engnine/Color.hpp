#pragma once

// TODO: Add values range automatically correction

#include <SFML/Graphics/Color.hpp>
#include <stdexcept>

#include "NumberUtils.hpp"
#include "base/MarshalUtils.hpp"
#include "engnine/EngineBase.hpp"
#include "integrator/Convert.hpp"

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

  Color(VALUE rbObj = Qnil) :
      Color(rbObj, 0, 0, 0) { }

  Color(Color *color) :
      Color(Qnil, color->red, color->green, color->blue, color->alpha) { }

  Color(double red, double green, double blue, double alpha) :
      Color(Qnil, red, green, blue, alpha) { }

  Color(VALUE rbObj, double red, double green, double blue, double alpha) :
      EngineBase(rbObj)
  {
    set(red, green, blue, alpha);
    syncSfColor();
  }

  ~Color()
  {
  }

  void initInstanceVars()
  {
    syncInstanceVars();
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
    syncInstanceVars();
    syncSfColor();
    return *this;
  }

  void set(double r, double g, double b, double a = 255)
  {
    red = Num::clamp<double>(r, 0.0, 255.0);
    green = Num::clamp<double>(g, 0.0, 255.0);
    blue = Num::clamp<double>(b, 0.0, 255.0);
    alpha = Num::clamp<double>(a, 0.0, 255.0);
    syncInstanceVars();
    syncSfColor();
    markAsDirty();
  }

  VALUE setter_red(VALUE value)
  {
    double v = Convert::toCDouble2(value);
    red = Num::clamp<double>(v, 0.0, 255.0);
    value = Convert::toRubyDouble(red);
    setInstanceVar("@red", value);
    sfColor.r = red;
    markAsDirty();
    return value;
  }

  VALUE setter_green(VALUE value)
  {
    double v = Convert::toCDouble2(value);
    green = Num::clamp<double>(v, 0.0, 255.0);
    value = Convert::toRubyDouble(green);
    setInstanceVar("@green", value);
    sfColor.g = green;
    markAsDirty();
    return value;
  }

  VALUE setter_blue(VALUE value)
  {
    double v = Convert::toCDouble2(value);
    blue = Num::clamp<double>(v, 0.0, 255.0);
    value = Convert::toRubyDouble(blue);
    setInstanceVar("@blue", value);
    sfColor.b = blue;
    markAsDirty();
    return value;
  }

  VALUE setter_alpha(VALUE value)
  {
    double v = Convert::toCDouble2(value);
    alpha = Num::clamp<double>(v, 0.0, 255.0);
    value = Convert::toRubyDouble(alpha);
    setInstanceVar("@alpha", value);
    sfColor.a = alpha;
    markAsDirty();
    return value;
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

  void syncSfColor()
  {
    sfColor.r = red;
    sfColor.b = blue;
    sfColor.g = green;
    sfColor.a = alpha;
  }

  void syncInstanceVars()
  {
    setInstanceVar("@red", Convert::toRubyDouble(red));
    setInstanceVar("@green", Convert::toRubyDouble(green));
    setInstanceVar("@blue", Convert::toRubyDouble(blue));
    setInstanceVar("@alpha", Convert::toRubyDouble(alpha));
  }
};

}  // namespace Eng