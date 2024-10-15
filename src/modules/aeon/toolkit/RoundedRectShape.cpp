#include "./RoundedRectShape.h"

#include <cmath>

namespace ae {

/*
  ------------------------------------------------------

    ⇩⇩⇩⇩⇩⇩⇩⇩⇩ Public ⇩⇩⇩⇩⇩⇩⇩⇩⇩

  ------------------------------------------------------
*/

RoundedRectShape::RoundedRectShape(const Rect& bounds, float radius) :
    RoundedRectShape(
      bounds.x.get(),
      bounds.y.get(),
      bounds.width.get(),
      bounds.height.get(),
      radius
    )
{
}

RoundedRectShape::RoundedRectShape(float x, float y, float width, float height, float radius) :
    positionVal(x, y),
    sizeVal(width, height),
    dirtyStyle(false),
    dirtyPosition(false),
    dirtyPoints(false)
{
  style.radius = radius;
  setPoints();
  setPosition();
  setStyle();
}

void RoundedRectShape::drawTo(RenderTarget& target)
{
  refreshValues();
  target.draw(shape);
}

/*
  Position
*/

const Vector2f& RoundedRectShape::position() const
{
  return positionVal;
}

void RoundedRectShape::position(float x, float y)
{
  positionVal.x = x;
  positionVal.y = y;
  dirtyPosition = true;
}

void RoundedRectShape::position(const Vector2f& value)
{
  positionVal = value;
  dirtyPosition = true;
}

float RoundedRectShape::x()
{
  return positionVal.x;
}

void RoundedRectShape::x(float value)
{
  positionVal.x = value;
  dirtyPosition = true;
}

float RoundedRectShape::y()
{
  return positionVal.y;
}

void RoundedRectShape::y(float value)
{
  positionVal.y = value;
  dirtyPosition = true;
}

/*
  Size
*/

const Vector2f& RoundedRectShape::size() const
{
  return sizeVal;
}

void RoundedRectShape::size(float width, float height)
{
  sizeVal.x = width;
  sizeVal.y = height;
  dirtyPoints = true;
}

void RoundedRectShape::size(const Vector2f& value)
{
  sizeVal = value;
  dirtyPoints = true;
}

float RoundedRectShape::width()
{
  return sizeVal.x;
}

void RoundedRectShape::width(float value)
{
  sizeVal.x = value;
  dirtyPoints = true;
}

float RoundedRectShape::height()
{
  return sizeVal.y;
}

void RoundedRectShape::height(float value)
{
  sizeVal.y = value;
  dirtyPoints = true;
}

/*
  Bounds
*/

// const Rect& RoundedRectShape::bounds() const { }

void RoundedRectShape::bounds(float x, float y, float width, float height)
{
  positionVal.x = x;
  positionVal.y = y;
  sizeVal.x = width;
  sizeVal.y = height;
  dirtyPosition = true;
  dirtyPoints = true;
}

void RoundedRectShape::bounds(const Vector2f& position, const Vector2f& size)
{
  positionVal = position;
  sizeVal = size;
  dirtyPosition = true;
  dirtyPoints = true;
}

void RoundedRectShape::bounds(const Rect& value)
{
  positionVal.x = value.x.get();
  positionVal.y = value.y.get();
  sizeVal.x = value.width.get();
  sizeVal.y = value.height.get();
  dirtyPosition = true;
  dirtyPoints = true;
}

/*
  Rounded radius
*/

float RoundedRectShape::radius()
{
  return style.radius;
}

void RoundedRectShape::radius(float radius)
{
  style.radius = radius;
  markDirtyPoints();
}

/*
  Mark dirty flags
*/

void RoundedRectShape::markDirtyStyle()
{
  dirtyStyle = true;
}

void RoundedRectShape::markDirtyPoints()
{
  dirtyPoints = true;
}

/*
  ------------------------------------------------------

    ⇩⇩⇩⇩⇩⇩⇩⇩⇩ Private ⇩⇩⇩⇩⇩⇩⇩⇩⇩

  ------------------------------------------------------
*/

void RoundedRectShape::refreshValues()
{
  if (dirtyStyle) {
    setStyle();
    dirtyStyle = false;
  }
  if (dirtyPosition) {
    setPosition();
    dirtyPosition = false;
  }
  if (dirtyPoints) {
    setPoints();
    dirtyPoints = false;
  }
}

void RoundedRectShape::setPosition()
{
  shape.setPosition(positionVal);
}

void RoundedRectShape::setStyle()
{
  shape.setOutlineThickness(style.borderSize);
  shape.setOutlineColor(style.borderColor.getSfColor());
  shape.setFillColor(style.bgColor.getSfColor());
}

void RoundedRectShape::setPoints()
{
  int points = 16;
  float radius = style.radius;
  Vector2f c(0, 0);
  int point = 0;
  float start = M_PI * 0.5;
  float end = M_PI * 0;

  shape.setPointCount(points * 4);

  for (int i = 0; i < points; i++) {
    float angle = start + (i * (end - start)) / points;
    float rad = angle * M_PI / 180.0f;
    float dx = std::cos(angle) * radius;
    float dy = std::sin(angle) * radius;
    c.x = sizeVal.x - radius + dx;
    c.y = radius - dy;
    shape.setPoint(point, Vector2f(c));
    point++;
  }

  start = M_PI * 2;
  end = M_PI * 1.5;
  for (int i = 0; i < points; i++) {
    float angle = start + (i * (end - start)) / points;
    float rad = angle * M_PI / 180.0f;
    float dx = std::cos(angle) * radius;
    float dy = std::sin(angle) * radius;
    c.x = sizeVal.x - radius + dx;
    c.y = sizeVal.y - radius - dy;
    shape.setPoint(point, Vector2f(c));
    point++;
  }

  start = M_PI * 1.5;
  end = M_PI * 1;
  for (int i = 0; i < points; i++) {
    float angle = start + (i * (end - start)) / points;
    float rad = angle * M_PI / 180.0f;
    float dx = std::cos(angle) * radius;
    float dy = std::sin(angle) * radius;
    c.x = radius + dx;
    c.y = sizeVal.y - radius - dy;
    shape.setPoint(point, Vector2f(c));
    point++;
  }

  start = M_PI * 1;
  end = M_PI * 0.5;
  for (int i = 0; i < points; i++) {
    float angle = start + (i * (end - start)) / points;
    float rad = angle * M_PI / 180.0f;
    float dx = std::cos(angle) * radius;
    float dy = std::sin(angle) * radius;
    c.x = radius + dx;
    c.y = radius - dy;
    shape.setPoint(point, Vector2f(c));
    point++;
  }
}

}  // namespace ae