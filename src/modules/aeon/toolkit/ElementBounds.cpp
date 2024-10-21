#include "aeon/toolkit/ElementBounds.h"

namespace ae {

/*
  ⇩⇩⇩ Constructor ⇩⇩⇩
*/

ElementBounds::ElementBounds(float x, float y, float width, float height) :
    xStart(x),
    yStart(y),
    _width(width),
    _height(height)
{
  recalculateBounds();
}

/*
  ⇩⇩⇩ Getters ands Setters ⇩⇩⇩
*/

float ElementBounds::x() const
{
  return xStart;
}

float ElementBounds::x(float value)
{
  xStart = value;
  positionVec.x = xStart;
  xEnd = xStart + _width;
  return xStart;
}

float ElementBounds::y() const
{
  return yStart;
}

float ElementBounds::y(float value)
{
  yStart = value;
  positionVec.y = yStart;
  yEnd = yStart + _height;
  return yStart;
}

float ElementBounds::width() const
{
  return _width;
}

float ElementBounds::width(float value)
{
  _width = value;
  sizeVec.x = _width;
  xEnd = xStart + _width;
  return _width;
}

float ElementBounds::height() const
{
  return _height;
}

float ElementBounds::height(float value)
{
  _height = value;
  sizeVec.y = value;
  yEnd = yStart + _height;
  return _height;
}

const Vector2f& ElementBounds::position() const
{
  return positionVec;
}

const Vector2f& ElementBounds::position(float x, float y)
{
  xStart = x;
  yStart = y;
  positionVec.x = xStart;
  positionVec.y = yStart;
  xEnd = xStart + _width;
  yEnd = yStart + _height;
  return positionVec;
}

const Vector2f& ElementBounds::position(const Vector2f& value)
{
  positionVec = value;
  xStart = positionVec.x;
  yStart = positionVec.y;
  xEnd = xStart + _width;
  yEnd = yStart + _height;
  return positionVec;
}

const Vector2f& ElementBounds::size() const
{
  return sizeVec;
}

const Vector2f& ElementBounds::size(float width, float height)
{
  _width = width;
  _height = height;
  sizeVec.x = _width;
  sizeVec.y = _height;
  xEnd = xStart + _width;
  yEnd = yStart + _height;
  return sizeVec;
}

const Vector2f& ElementBounds::size(const Vector2f& value)
{
  sizeVec = value;
  _width = sizeVec.x;
  _height = sizeVec.y;
  xEnd = xStart + _width;
  yEnd = yStart + _height;
  return sizeVec;
}

/*
  ⇩⇩⇩ Operators ⇩⇩⇩
*/

ElementBounds& ElementBounds::operator=(const ElementBounds& other)
{
  if (this == &other) {
    return *this;
  }

  xStart = other.xStart;
  yStart = other.yStart;
  _width = other._width;
  _height = other._height;
  recalculateBounds();
  return *this;
}

ElementBounds& ElementBounds::operator+=(const ElementBounds& other)
{
  xStart += other.xStart;
  yStart += other.yStart;
  _width += other._width;
  _height += other._height;
  recalculateBounds();
  return *this;
}

ElementBounds& ElementBounds::operator-=(const ElementBounds& other)
{
  xStart -= other.xStart;
  yStart -= other.yStart;
  _width -= other._width;
  _height -= other._height;
  recalculateBounds();
  return *this;
}

ElementBounds ElementBounds::operator+(const ElementBounds& other) const
{
  return ElementBounds(
    xStart + other.xStart,
    yStart + other.yStart,
    _width + other._width,
    _height + other._height
  );
}

ElementBounds ElementBounds::operator-(const ElementBounds& other) const
{
  return ElementBounds(
    xStart - other.xStart,
    yStart - other.yStart,
    _width - other._width,
    _height - other._height
  );
}

ElementBounds& ElementBounds::operator+=(float value)
{
  xStart += value;
  yStart += value;
  _width += value;
  _height += value;
  recalculateBounds();
  return *this;
}

ElementBounds& ElementBounds::operator-=(float value)
{
  xStart -= value;
  yStart -= value;
  _width -= value;
  _height -= value;
  recalculateBounds();
  return *this;
}

ElementBounds ElementBounds::operator+(float value) const
{
  return ElementBounds(
    xStart + value,
    yStart + value,
    _width + value,
    _height + value
  );
}

ElementBounds ElementBounds::operator-(float value) const
{
  return ElementBounds(
    xStart - value,
    yStart - value,
    _width - value,
    _height - value
  );
}

ElementBounds ElementBounds::operator/(float value) const
{
  return ElementBounds(
    xStart / value,
    yStart / value,
    _width / value,
    _height / value
  );
}

/*
  ⇩⇩⇩ Private ⇩⇩⇩
*/

void ElementBounds::recalculateBounds()
{
  positionVec.x = xStart;
  positionVec.y = yStart;

  sizeVec.x = _width;
  sizeVec.y = _height;

  xEnd = xStart + _width;
  yEnd = yStart + _height;
}

}  // namespace ae