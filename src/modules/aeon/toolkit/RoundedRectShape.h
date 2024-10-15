#pragma once

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "engnine/Rect.hpp"
#include "aeon/window/AeonStylesheet.h"

namespace ae {

using Eng::Rect;
using sf::ConvexShape;
using sf::RenderTarget;
using sf::Vector2f;

struct RoundedRectShape {
  AeonStylesheet style;

  RoundedRectShape(const Rect& bounds, float radius = 0);
  RoundedRectShape(float x, float y, float width, float height, float radius = 0);

  void drawTo(RenderTarget& target);

  const Vector2f& position() const;
  void position(float x, float y);
  void position(const Vector2f& value);

  float x();
  void x(float value);
  float y();
  void y(float value);

  const Vector2f& size() const;
  void size(float width, float height);
  void size(const Vector2f& value);

  float width();
  void width(float value);
  float height();
  void height(float value);

  // const Rect& bounds() const;
  void bounds(float x, float y, float width, float height);
  void bounds(const Vector2f& position, const Vector2f& size);
  void bounds(const Rect& value);

  float radius();
  void radius(float radius);

  void markDirtyStyle();
  void markDirtyPoints();

 private:
  Vector2f positionVal;
  Vector2f sizeVal;
  ConvexShape shape;
  bool dirtyStyle;
  bool dirtyPosition;
  bool dirtyPoints;

  void refreshValues();
  void setPoints();
  void setPosition();
  void setStyle();
};

}  // namespace ae