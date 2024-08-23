#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <stdexcept>

#include "MarshalUtils.hpp"
#include "engnine/EngineBase.hpp"
#include "engnine/base/Prop.hpp"
#include "engnine/base/PropNumber.hpp"

namespace Eng {

class Rect : public EngineBase {
 public:
  static constexpr int SERIAL_SIZE = 16;

  static Rect *deserialize(const char *data, int len)
  {
    if (len != SERIAL_SIZE) {
      throw std::runtime_error("Marshal error: Rect has a bad file format");
    }

    float x = MarshalUtils::readFloat(&data);
    float y = MarshalUtils::readFloat(&data);
    int width = MarshalUtils::readInt32(&data);
    int height = MarshalUtils::readInt32(&data);

    Rect *rect = new Rect(x, y, width, height);
    return rect;
  }

  void serialize(char *buffer) const
  {
    MarshalUtils::writeFloat(&buffer, x.get());
    MarshalUtils::writeFloat(&buffer, y.get());
    MarshalUtils::writeInt32(&buffer, width.get());
    MarshalUtils::writeInt32(&buffer, height.get());
  }

  PropNumber<double> x;
  PropNumber<double> y;
  PropNumber<int> width;
  PropNumber<int> height;

  Rect(double x, double y, int width, int height) :
      Rect(Qnil, x, y, width, height) { }

  Rect(Rect *rect) :
      Rect(Qnil, rect->x.get(), rect->y.get(), rect->width.get(), rect->height.get()) { }

  Rect(VALUE rbObj, double x, double y, int width, int height) :
      EngineBase(rbObj),

      x(Prop::Float("@x", x, this)),
      y(Prop::Float("@y", y, this)),
      width(Prop::FixNum("@width", width, this)),
      height(Prop::FixNum("@height", height, this))
  {
    if (hasRbObj()) {
      bindRubyVars();
    }
  }

  bool operator==(const Rect &other) const
  {
    return x == other.x && y == other.y && width == other.width && height == other.height;
  }

  Rect &operator=(const Rect &other)
  {
    if (this == &other) {
      return *this;
    }

    x = other.x;
    y = other.y;
    width = other.width;
    height = other.height;
    markAsDirty();

    return *this;
  }

  ~Rect()
  {
  }

  void bindRubyVars()
  {
    x.setInstanceVar();
    y.setInstanceVar();
    width.setInstanceVar();
    height.setInstanceVar();
  }

  void setVALUES(VALUE xVal, VALUE yVal, VALUE widthVal, VALUE heightVal)
  {
    x.setVALUE(xVal);
    y.setVALUE(yVal);
    width.setVALUE(widthVal);
    height.setVALUE(heightVal);
  }

  void set(double x, double y, int width, int height)
  {
    this->x.set(x);
    this->y.set(y);
    this->width.set(width);
    this->height.set(height);
  }

  void method_empty()
  {
    width.set(0);
    height.set(0);
  }

  inline bool isEmpty() const
  {
    return width == 0u || height == 0u;
  }

  inline sf::IntRect sfRect() const
  {
    return sf::IntRect(x.get(), y.get(), width.get(), height.get());
  }
};

}  // namespace Eng