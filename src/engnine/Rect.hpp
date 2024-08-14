#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <stdexcept>

#include "MarshalUtils.hpp"
#include "engnine/EngineBase.hpp"

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

  Rect(float _x, float _y, unsigned int _width, unsigned int _height) :
      dirty(false)
  {
    x = _x;
    y = _y;
    width = _width;
    height = _height;
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

  void set(int _x, int _y, int _width, int _height)
  {
    if (_x == x && y == _y && _width == width && _height == height) {
      return;
    }
    x = _x;
    y = _y;
    width = _width;
    height = _height;
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

  void setter_x(int v)
  {
    if (x == v) {
      return;
    }
    x = v;
    dirty = true;
  }

  void setter_y(int v)
  {
    if (y == v) {
      return;
    }
    y = v;
    dirty = true;
  }

  void setter_width(int v)
  {
    if (width == v) {
      return;
    }
    width = v;
    dirty = true;
  }

  void setter_height(int v)
  {
    if (height == v) {
      return;
    }
    height = v;
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
  unsigned int width;
  unsigned int height;
};

}  // namespace Eng