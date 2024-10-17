#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "AppDefs.h"
#include "aeon/window/AeonStyleSheet.h"
#include "engnine/Rect.hpp"

namespace ae {

using app::String;
using Eng::Rect;
using sf::ConvexShape;
using sf::RenderTarget;
using sf::Vector2f;

struct RoundedRectShape {
  RoundedRectShape(float radius = 0);
  RoundedRectShape(const Rect& bounds, float radius = 0);
  RoundedRectShape(float x, float y, float width, float height, float radius = 0);

  void drawTo(RenderTarget& target);

  const Vector2f& position() const;
  void position(float x, float y);
  void position(const Vector2f& value);

  float x() const;
  void x(float value);

  float y() const;
  void y(float value);

  const Vector2f& size() const;
  void size(float width, float height);
  void size(const Vector2f& value);

  float width() const;
  void width(float value);

  float height() const;
  void height(float value);

  void bounds(float x, float y, float width, float height);
  void bounds(const Vector2f& position, const Vector2f& size);
  void bounds(const Rect& value);

  float radius() const;
  void radius(float radius);

  float borderSize() const;
  void borderSize(float value);

  const sf::Color& borderColor() const;
  void borderColor(const String& hex);
  void borderColor(const Eng::Color& value);
  void borderColor(const sf::Color& value);

  const sf::Color& fillColor() const;
  void fillColor(const String& hex);
  void fillColor(const Eng::Color& value);
  void fillColor(const sf::Color& value);

 private:
  Vector2f positionVal;
  Vector2f sizeVal;
  float radiusValue;
  ConvexShape shape;
  bool dirtyPosition;
  bool dirtyPoints;

  void refreshValues();
  void setPoints();
  void setPosition();
};

}  // namespace ae