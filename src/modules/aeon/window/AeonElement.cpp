
#include "aeon/window/AeonElement.h"

#include <cstdint>

#include "aeon/enums/AeonElementState.h"
#include "aeon/toolkit/ElementBounds.h"
#include "aeon/window/AeonStyleSheet.h"

namespace ae {

AeonElement::AeonElement(const ElementBounds& bounds) :
    bounds(bounds),
    states(static_cast<uint8_t>(AeonElementState::DEFAULT)),
    dirtyBounds(false),
    dirtyState(false),
    dirtyStyle(false),
    sizeUndefined(true),
    focusable(true)
{
}

AeonElement::AeonElement(const ElementBounds& bounds, const AeonStyleSheetBase& defaultStyle) :
    bounds(bounds),
    defaultStyle(defaultStyle),
    states(static_cast<uint8_t>(AeonElementState::DEFAULT)),
    dirtyBounds(false),
    dirtyState(false),
    dirtyStyle(false),
    sizeUndefined(true),
    focusable(true)
{
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
  if (!bounds.isEmpty()) {
    sizeUndefined = true;
  }
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
  sizeUndefined = false;
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

void AeonElement::addState(AeonElementState state)
{
  states |= static_cast<uint8_t>(state);
  dirtyState = true;
}

void AeonElement::removeState(AeonElementState state)
{
  states &= ~static_cast<uint8_t>(state);
  dirtyState = true;
}

bool AeonElement::hasState(AeonElementState state) const
{
  return states & static_cast<uint8_t>(state);
}

void AeonElement::clearState()
{
  states = static_cast<uint8_t>(AeonElementState::DEFAULT);
  dirtyState = true;
}

const AeonStyleSheet& AeonElement::getStyle()
{
  return defaultStyle;
}

void AeonElement::setStyle(const AeonStyleSheet& style)
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

const AeonStyleSheet& AeonElement::getStateStyle(AeonElementState state)
{
  return stateStyles[state];
}

void AeonElement::setStateStyle(AeonElementState state, const AeonStyleSheet& style)
{
  AeonStyleSheet& target = stateStyles[state];
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

AeonStyleSheet& AeonElement::getMutableStyle()
{
  dirtyState = true;
  return defaultStyle;
}

AeonStyleSheet& AeonElement::getMutableStyle(AeonElementState state)
{
  dirtyState = true;
  return stateStyles[state];
}

AeonStyleSheet& AeonElement::getMutableStyle(const String& stateName)
{
  if (stateName == "hover") {
    return getMutableStyle(AeonElementState::HOVER);
  } else if (stateName == "focus")
    return getMutableStyle(AeonElementState::FOCUS);
  else if (stateName == "focusVisible")
    return getMutableStyle(AeonElementState::FOCUS_VISIBLE);
  else if (stateName == "clicked")
    return getMutableStyle(AeonElementState::CLICKED);
  else
    return getMutableStyle(AeonElementState::DEFAULT);
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