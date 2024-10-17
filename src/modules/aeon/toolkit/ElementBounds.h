#pragma once

#include <SFML/System/Vector2.hpp>

namespace ae {

using sf::Vector2f;

struct ElementBounds {
  ElementBounds(float x = 0, float y = 0, float width = 0, float height = 0);

  float x() const;
  float x(float value);
  float y() const;
  float y(float value);

  float w() const;
  float w(float value);
  float h() const;
  float h(float value);

  float width() const;
  float width(float value);
  float height() const;
  float height(float value);

  const Vector2f& position() const;
  const Vector2f& position(float x, float y);
  const Vector2f& position(const Vector2f& value);

  const Vector2f& size() const;
  const Vector2f& size(const Vector2f& value);

  inline bool isEmpty() const
  {
    return wValue == 0 || hValue == 0;
  }

  inline bool intersects(float x, float y) const
  {
    return wValue == 0 || hValue == 0;
  }

  inline bool operator==(const ElementBounds& other) const
  {
    return xValue == other.xValue && yValue == other.yValue
           && wValue == other.wValue && hValue == other.hValue;
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

 private:
  float xValue;
  float yValue;
  float wValue;
  float hValue;
  float xEndValue;
  float yEndValue;
  Vector2f positionValue;
  Vector2f sizeValue;

  void recalculateBounds();
};

}  // namespace ae