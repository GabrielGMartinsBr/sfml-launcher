#include "./RoundedRectShape.h"

#include <cmath>

#include "aeon/toolkit/ColorParser.hpp"

namespace ae {

/*
  ------------------------------------------------------

    ⇩⇩⇩⇩⇩⇩⇩⇩⇩ Public ⇩⇩⇩⇩⇩⇩⇩⇩⇩

  ------------------------------------------------------
*/

RoundedRectShape::RoundedRectShape(float radius) :
    RoundedRectShape(0, 0, 0, 0, radius)
{
}

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
    radiusValue(radius),
    dirtyPosition(false),
    dirtyPoints(false)
{
  setPoints();
  setPosition();
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

float RoundedRectShape::x() const
{
  return positionVal.x;
}

void RoundedRectShape::x(float value)
{
  positionVal.x = value;
  dirtyPosition = true;
}

float RoundedRectShape::y() const
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
  sizeVal.x = value.x;
  sizeVal.y = value.y;
  dirtyPoints = true;
}

float RoundedRectShape::width() const
{
  return sizeVal.x;
}

void RoundedRectShape::width(float value)
{
  sizeVal.x = value;
  dirtyPoints = true;
}

float RoundedRectShape::height() const
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

float RoundedRectShape::radius() const
{
  return radiusValue;
}

void RoundedRectShape::radius(float radius)
{
  if (radiusValue == radius) {
    return;
  }
  radiusValue = radius;
  dirtyPoints = true;
}

/*
  Style
*/

// Border size

float RoundedRectShape::borderSize() const
{
  return shape.getOutlineThickness();
}

void RoundedRectShape::borderSize(float value)
{
  shape.setOutlineThickness(value);
}

// Border color

const sf::Color& RoundedRectShape::borderColor() const
{
  return shape.getOutlineColor();
}

void RoundedRectShape::borderColor(const String& hex)
{
  borderColor(ColorParser::hexToSfmlColor(hex));
}

void RoundedRectShape::borderColor(const Eng::Color& value)
{
  borderColor(value.getSfColor());
}

void RoundedRectShape::borderColor(const sf::Color& value)
{
  shape.setOutlineColor(value);
}

// Fill color

const sf::Color& RoundedRectShape::fillColor() const
{
  return shape.getFillColor();
}

void RoundedRectShape::fillColor(const String& hex)
{
  fillColor(ColorParser::hexToSfmlColor(hex));
}

void RoundedRectShape::fillColor(const Eng::Color& value)
{
  fillColor(value.getSfColor());
}

void RoundedRectShape::fillColor(const sf::Color& value)
{
  shape.setFillColor(value);
}

/*
  ------------------------------------------------------

    ⇩⇩⇩⇩⇩⇩⇩⇩⇩ Private ⇩⇩⇩⇩⇩⇩⇩⇩⇩

  ------------------------------------------------------
*/

void RoundedRectShape::refreshValues()
{
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

void RoundedRectShape::setPoints()
{
  if (radiusValue == 0) {
    shape.setPointCount(4);
    shape.setPoint(0, Vector2f(0, 0));
    shape.setPoint(1, Vector2f(sizeVal.x, 0));
    shape.setPoint(2, Vector2f(sizeVal.x, sizeVal.y));
    shape.setPoint(3, Vector2f(0, sizeVal.y));
    return;
  }

  int points = 16;
  Vector2f c(0, 0);
  int point = 0;
  float start = M_PI * 0.5;
  float end = M_PI * 0;

  shape.setPointCount(points * 4);

  for (int i = 0; i < points; i++) {
    float angle = start + (i * (end - start)) / points;
    float rad = angle * M_PI / 180.0f;
    float dx = std::cos(angle) * radiusValue;
    float dy = std::sin(angle) * radiusValue;
    c.x = sizeVal.x - radiusValue + dx;
    c.y = radiusValue - dy;
    shape.setPoint(point, c);
    point++;
  }

  start = M_PI * 2;
  end = M_PI * 1.5;
  for (int i = 0; i < points; i++) {
    float angle = start + (i * (end - start)) / points;
    float rad = angle * M_PI / 180.0f;
    float dx = std::cos(angle) * radiusValue;
    float dy = std::sin(angle) * radiusValue;
    c.x = sizeVal.x - radiusValue + dx;
    c.y = sizeVal.y - radiusValue - dy;
    shape.setPoint(point, c);
    point++;
  }

  start = M_PI * 1.5;
  end = M_PI * 1;
  for (int i = 0; i < points; i++) {
    float angle = start + (i * (end - start)) / points;
    float rad = angle * M_PI / 180.0f;
    float dx = std::cos(angle) * radiusValue;
    float dy = std::sin(angle) * radiusValue;
    c.x = radiusValue + dx;
    c.y = sizeVal.y - radiusValue - dy;
    shape.setPoint(point, c);
    point++;
  }

  start = M_PI * 1;
  end = M_PI * 0.5;
  for (int i = 0; i < points; i++) {
    float angle = start + (i * (end - start)) / points;
    float rad = angle * M_PI / 180.0f;
    float dx = std::cos(angle) * radiusValue;
    float dy = std::sin(angle) * radiusValue;
    c.x = radiusValue + dx;
    c.y = radiusValue - dy;
    shape.setPoint(point, c);
    point++;
  }
}

}  // namespace ae