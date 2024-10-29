#pragma once

#include <SFML/System/Vector2.hpp>

namespace ae {

using sf::Vector2f;

struct ElementBounds {
  ElementBounds(const Vector2f& position, const Vector2f& size);

  ElementBounds(float x = 0, float y = 0, float width = 0, float height = 0);

  float x() const;
  float x(float value);
  float y() const;
  float y(float value);
  float width() const;
  float width(float value);
  float height() const;
  float height(float value);

  inline float endX() const { return xEnd; }
  inline float endY() const { return yEnd; }

  const Vector2f& position() const;
  const Vector2f& position(float x, float y);
  const Vector2f& position(const Vector2f& value);

  const Vector2f& size() const;
  const Vector2f& size(float width, float height);
  const Vector2f& size(const Vector2f& value);

  inline bool isEmpty() const
  {
    return _width == 0 || _height == 0;
  }

  inline bool intersects(float x, float y) const
  {
    return !isEmpty() && x >= xStart && x <= xEnd && y >= yStart && y < yEnd;
  }

  inline bool intersects(float x, float y, float border) const
  {
    return !isEmpty() && x >= (xStart - border) && x <= (xEnd + border)
           && y >= (yStart - border) && y < (yEnd + border);
  }

  inline bool operator==(const ElementBounds& other) const
  {
    return xStart == other.xStart && yStart == other.yStart
           && _width == other._width && _height == other._height;
  }

  ElementBounds& operator=(const ElementBounds& other);

  ElementBounds& operator+=(const ElementBounds& other);
  ElementBounds& operator-=(const ElementBounds& other);
  ElementBounds operator+(const ElementBounds& other) const;
  ElementBounds operator-(const ElementBounds& other) const;

  ElementBounds& operator+=(float value);
  ElementBounds& operator-=(float value);
  ElementBounds operator+(float value) const;
  ElementBounds operator-(float value) const;
  ElementBounds operator/(float value) const;

  inline ElementBounds& operator/=(const Vector2f& other)
  {
    xStart /= other.x;
    yStart /= other.y;
    _width /= other.x;
    _height /= other.y;
    recalculateBounds();
    return *this;
  }


 private:
  float xStart;
  float yStart;
  float _width;
  float _height;
  float xEnd;
  float yEnd;
  Vector2f positionVec;
  Vector2f sizeVec;

  void recalculateBounds();
};

}  // namespace ae