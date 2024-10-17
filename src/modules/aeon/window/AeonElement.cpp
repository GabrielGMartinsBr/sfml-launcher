
#include "aeon/window/AeonElement.h"

#include "aeon/toolkit/ElementBounds.h"

namespace ae {

AeonElement::AeonElement(const ElementBounds& bounds, const AeonStyleSheet& style) :
    bounds(bounds),
    defaultStyle(style),
    elementState(AeonElementState::DEFAULT),
    dirtyBounds(false),
    dirtyStyle(false),
    focusable(true)
{
}

inline bool AeonElement::intersects(float x, float y) const
{
  return bounds.intersects(x, y);
}

void AeonElement::drawTo(RenderTarget& target) { }

/*
  ⇩⇩⇩ Getters And Setters ⇩⇩⇩
*/

const ElementBounds& AeonElement::getBounds()
{
  return bounds;
}

void AeonElement::setBounds(const ElementBounds& value)
{
  bounds = value;
}

void AeonElement::setPosition(float x, float y)
{
  bounds.position(x, y);
  dirtyBounds = true;
}

void AeonElement::setSize(float width, float height)
{
  bounds.width(width);
  bounds.height(height);
  dirtyBounds = true;
}

float AeonElement::x()
{
  return bounds.x();
}

float AeonElement::y()
{
  return bounds.y();
}

void AeonElement::x(float value)
{
  bounds.x(value);
  dirtyBounds = true;
}

void AeonElement::y(float value)
{
  bounds.y(value);
  dirtyBounds = true;
}

const AeonStyleSheet& AeonElement::getStyle()
{
  return defaultStyle;
}

void AeonElement::setStyle(const AeonPartialStyleSheet& style)
{
  if (style.ringSize) defaultStyle.ringSize = *style.ringSize;
  if (style.ringOffset) defaultStyle.ringOffset = *style.ringOffset;
  if (style.borderSize) defaultStyle.borderSize = *style.borderSize;
  if (style.radius) defaultStyle.radius = *style.radius;
  if (style.fontSize) defaultStyle.fontSize = *style.fontSize;
  if (style.padding) defaultStyle.padding = *style.padding;
  if (style.fontName) defaultStyle.fontName = *style.fontName;
  if (style.ringColor) defaultStyle.ringColor = *style.ringColor;
  if (style.borderColor) defaultStyle.borderColor = *style.borderColor;
  if (style.bgColor) defaultStyle.bgColor = *style.bgColor;
  if (style.textColor) defaultStyle.textColor = *style.textColor;
  dirtyStyle = true;
}

const AeonPartialStyleSheet& AeonElement::getStateStyle(AeonElementState state)
{
  return stateStyles[state];
}

void AeonElement::setStateStyle(AeonElementState state, const AeonPartialStyleSheet& style)
{
  AeonPartialStyleSheet target = stateStyles[state];
  if (style.ringSize) target.ringSize = *style.ringSize;
  if (style.ringOffset) target.ringOffset = *style.ringOffset;
  if (style.borderSize) target.borderSize = *style.borderSize;
  if (style.radius) target.radius = *style.radius;
  if (style.fontSize) target.fontSize = *style.fontSize;
  if (style.padding) target.padding = *style.padding;
  if (style.fontName) target.fontName = *style.fontName;
  if (style.ringColor) target.ringColor = *style.ringColor;
  if (style.borderColor) target.borderColor = *style.borderColor;
  if (style.bgColor) target.bgColor = *style.bgColor;
  if (style.textColor) target.textColor = *style.textColor;
}

const AeonElementState& AeonElement::getState()
{
  return elementState;
}

void AeonElement::setState(const AeonElementState& value)
{
  elementState = value;
}

bool AeonElement::isFocusable() const
{
  return focusable;
}

void AeonElement::setFocusable(bool value)
{
  focusable = true;
}

}  // namespace ae