#include "aeon/toolkit/ElementBounds.h"

namespace ae {

/*
  ⇩⇩⇩ Constructor ⇩⇩⇩
*/

ElementBounds::ElementBounds(float x, float y, float width, float height) :
    xValue(x),
    yValue(y),
    wValue(width),
    hValue(height)
{
  recalculateBounds();
}

/*
  ⇩⇩⇩ Getters ands Setters ⇩⇩⇩
*/

float ElementBounds::x() const
{
  return xValue;
}

float ElementBounds::x(float value)
{
  xValue = value;
  xEndValue = xValue + wValue;
  positionValue.x = xValue;
  return xValue;
}

float ElementBounds::y() const
{
  return yValue;
}

float ElementBounds::y(float value)
{
  yValue = value;
  yEndValue = yValue + hValue;
  positionValue.y = yValue;
  return yValue;
}

float ElementBounds::w() const
{
  return wValue;
}

float ElementBounds::w(float value)
{
  wValue = value;
  xEndValue = xValue + wValue;
  sizeValue.x = wValue;
  return wValue;
}

float ElementBounds::h() const
{
  return hValue;
}

float ElementBounds::h(float value)
{
  hValue = value;
  yEndValue = yValue + hValue;
  sizeValue.y = hValue;
  return hValue;
}

float ElementBounds::width() const
{
  return w();
}

float ElementBounds::width(float value)
{
  wValue = value;
  xEndValue = xValue + wValue;
  sizeValue.x = value;
  return w(value);
}

float ElementBounds::height() const
{
  return h();
}

float ElementBounds::height(float value)
{
  hValue = value;
  yEndValue = xValue + hValue;
  sizeValue.y = value;
  return h(value);
}

const Vector2f& ElementBounds::position() const
{
  return positionValue;
}

const Vector2f& ElementBounds::position(float x, float y)
{
  xValue = x;
  yValue = y;
  xEndValue = xValue + wValue;
  yEndValue = yValue + hValue;
  positionValue.x = xValue;
  positionValue.y = yValue;
  return positionValue;
}

const Vector2f& ElementBounds::position(const Vector2f& value)
{
  positionValue = value;
  xValue = positionValue.x;
  yValue = positionValue.y;
  xEndValue = xValue + wValue;
  yEndValue = yValue + hValue;
  return positionValue;
}

const Vector2f& ElementBounds::size() const
{
  return sizeValue;
}

const Vector2f& ElementBounds::size(const Vector2f& value)
{
  sizeValue = value;
  wValue = sizeValue.x;
  hValue = sizeValue.y;
  xEndValue = xValue + wValue;
  yEndValue = yValue + hValue;
  return sizeValue;
}

/*
  ⇩⇩⇩ Operators ⇩⇩⇩
*/

ElementBounds& ElementBounds::operator=(const ElementBounds& other)
{
  if (this == &other) {
    return *this;
  }

  xValue = other.xValue;
  yValue = other.yValue;
  wValue = other.wValue;
  hValue = other.hValue;
  recalculateBounds();
  return *this;
}

ElementBounds& ElementBounds::operator+=(const ElementBounds& other)
{
  xValue += other.xValue;
  yValue += other.yValue;
  wValue += other.wValue;
  hValue += other.hValue;
  recalculateBounds();
  return *this;
}

ElementBounds& ElementBounds::operator-=(const ElementBounds& other)
{
  xValue -= other.xValue;
  yValue -= other.yValue;
  wValue -= other.wValue;
  hValue -= other.hValue;
  recalculateBounds();
  return *this;
}

ElementBounds ElementBounds::operator+(const ElementBounds& other) const
{
  return ElementBounds(
    xValue + other.xValue,
    yValue + other.yValue,
    wValue + other.wValue,
    hValue + other.hValue
  );
}

ElementBounds ElementBounds::operator-(const ElementBounds& other) const
{
  return ElementBounds(
    xValue - other.xValue,
    yValue - other.yValue,
    wValue - other.wValue,
    hValue - other.hValue
  );
}

ElementBounds& ElementBounds::operator+=(float value)
{
  xValue += value;
  yValue += value;
  wValue += value;
  hValue += value;
  recalculateBounds();
  return *this;
}

ElementBounds& ElementBounds::operator-=(float value)
{
  xValue -= value;
  yValue -= value;
  wValue -= value;
  hValue -= value;
  recalculateBounds();
  return *this;
}

ElementBounds ElementBounds::operator+(float value) const
{
  return ElementBounds(
    xValue + value,
    yValue + value,
    wValue + value,
    hValue + value
  );
}

ElementBounds ElementBounds::operator-(float value) const
{
  return ElementBounds(
    xValue - value,
    yValue - value,
    wValue - value,
    hValue - value
  );
}

/*
  ⇩⇩⇩ Private ⇩⇩⇩
*/

void ElementBounds::recalculateBounds()
{
  xEndValue = xValue + wValue;
  yEndValue = yValue + hValue;

  positionValue.x = xValue;
  positionValue.y = yValue;

  sizeValue.x = wValue;
  sizeValue.y = hValue;
}

}  // namespace ae