#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <stdexcept>

#include "MarshalUtils.hpp"
#include "engnine/EngineBase.hpp"
#include "integrator/Convert.hpp"

namespace Eng {

class Rect : public EngineBase {
 public:
  static constexpr int SERIAL_SIZE = 16;

  static Rect *deserialize(const char *data, int len)
  {
    if (len != SERIAL_SIZE) {
      throw std::runtime_error("Marshal error: Rect has a bad file format");
    }

    int x = MarshalUtils::readInt32(&data);
    int y = MarshalUtils::readInt32(&data);
    int width = MarshalUtils::readInt32(&data);
    int height = MarshalUtils::readInt32(&data);

    Rect *rect = new Rect(x, y, width, height);
    return rect;
  }

  void serialize(char *buffer) const
  {
    MarshalUtils::writeInt32(&buffer, x);
    MarshalUtils::writeInt32(&buffer, y);
    MarshalUtils::writeInt32(&buffer, width);
    MarshalUtils::writeInt32(&buffer, height);
  }

  Rect(float x, float y, int width, int height) :
      Rect(Qnil, x, y, width, height) { }

  Rect(VALUE rbObj, float x, float y, int width, int height) :
      EngineBase(rbObj),
      dirty(false),
      x(x),
      y(y),
      width(width),
      height(height)
  {
    if (rbObj != Qnil) {
      initialize();
    }
  }

  Rect(Rect *_rect) :
      dirty(false)
  {
    x = _rect->x;
    y = _rect->y;
    width = _rect->width;
    height = _rect->height;
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
    dirty = true;

    return *this;
  }

  ~Rect()
  {
  }

  void initialize()
  {
    rb_iv_set(rbObj, "@x", Convert::toRubyNumber(x));
    rb_iv_set(rbObj, "@y", Convert::Convert::toRubyNumber(y));
    rb_iv_set(rbObj, "@width", Convert::toRubyNumber(width));
    rb_iv_set(rbObj, "@height", Convert::toRubyNumber(height));
  }

  void set(VALUE xVal, VALUE yVal, VALUE widthVal, VALUE heightVal)
  {
    int _x = Convert::toCInt(xVal);
    int _y = Convert::toCInt(yVal);
    int _width = Convert::toCInt(widthVal);
    int _height = Convert::toCInt(heightVal);

    if (_x == x && y == _y && _width == width && _height == height) {
      return;
    }
    x = _x;
    y = _y;
    width = _width;
    height = _height;
    if (hasRbObj()) {
      rb_iv_set(rbObj, "@x", xVal);
      rb_iv_set(rbObj, "@y", yVal);
      rb_iv_set(rbObj, "@width", widthVal);
      rb_iv_set(rbObj, "@height", heightVal);
    }

    dirty = true;
  }

  inline int getter_x() const
  {
    return x;
  }

  inline int getter_y() const
  {
    return y;
  }

  inline int getter_width() const
  {
    return width;
  }

  inline int getter_height() const
  {
    return height;
  }

  void setter_x(VALUE value)
  {
    int v = Convert::toCInt(value);
    if (x == v) return;
    x = v;
    if (hasRbObj()) {
      rb_iv_set(rbObj, "@x", value);
    }
    dirty = true;
  }

  void setter_y(VALUE value)
  {
    int v = Convert::toCInt(value);
    if (y == v) return;
    y = v;
    if (hasRbObj()) {
      rb_iv_set(rbObj, "@y", value);
    }
    dirty = true;
  }

  void setter_width(VALUE value)
  {
    int v = Convert::toCUnsignedInt(value);
    if (width == v) return;
    width = v;
    if (hasRbObj()) {
      rb_iv_set(rbObj, "@width", value);
    }
    dirty = true;
  }

  void setter_height(VALUE value)
  {
    int v = Convert::toCUnsignedInt(value);
    if (height == v) return;
    height = v;
    if (hasRbObj()) {
      rb_iv_set(rbObj, "@height", value);
    }
    dirty = true;
  }

  void method_empty()
  {
    width = 0;
    height = 0;
    dirty = true;
  }

  inline bool isEmpty() const
  {
    return width == 0 || height == 0;
  }

  inline sf::IntRect sfRect() const
  {
    return sf::IntRect(x, y, width, height);
  }

  void markAsDirty()
  {
    dirty = true;
  }

  void markAsClean()
  {
    dirty = false;
  }

  inline bool isDirty() const
  {
    return dirty;
  }


 private:
  bool dirty;
  float x;
  float y;
  int width;
  int height;
};

}  // namespace Eng