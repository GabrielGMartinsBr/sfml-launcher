#pragma once

#include <SFML/Graphics/Rect.hpp>

#include "Log.hpp"
#include "engnine/EngineBase.hpp"

namespace Eng {

class Rect : public EngineBase {
 public:

  Rect(float _x, float _y, unsigned int _width, unsigned int _height) :
      dirty(false)
  {
    x = _x;
    y = _y;
    width = _width;
    height = _height;
  }

  Rect(Rect* _rect) :
      dirty(false)
  {
    x = _rect->x;
    y = _rect->y;
    width = _rect->width;
    height = _rect->height;
  }

  Rect& operator=(const Rect& other)
  {
    if (this == &other) {
      return *this;
    }

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