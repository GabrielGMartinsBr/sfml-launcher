#include "./AeonTextBoxElement.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>

#include "aeon/enums/AeonElementState.h"
#include "aeon/toolkit/ElementBounds.h"
#include "aeon/window/AeonStyleSheet.h"
#include "engnine/base/Fonts.h"

namespace ae {

static const float cursorBlinkTime = 0.5f;

AeonTextBoxElement::AeonTextBoxElement() :
    AeonElement(ElementBounds(), textBoxDefaultStyle),
    textFont(nullptr),
    fontSize(16),
    showCursor(true),
    dirtyTextValue(false)
{
  applyStyle();
  applyBounds();
  valueString = "TextBox works!";
  text.setString(valueString);
  cursorIndex = valueString.getSize();
}

void AeonTextBoxElement::handleAeonUpdate(ULong ts)
{
  timestamp = ts;
  if (timestamp - lastCursorBlinkTs > cursorBlinkTime) {
    showCursor = !showCursor;
    lastCursorBlinkTs = timestamp;
  }
}

void AeonTextBoxElement::drawTo(RenderTarget& target)
{
  refreshValues();
  shape.drawTo(target);
  target.draw(text);
  if (showCursor) {
    target.draw(cursorShape);
  }
}

void AeonTextBoxElement::flush()
{
  refreshValues();
}

const sf::String& AeonTextBoxElement::getValue()
{
  return valueString;
}

const sf::String& AeonTextBoxElement::setValue(const sf::String& value)
{
  valueString = value;
  dirtyTextValue = true;
  return valueString;
}

/*
  ⇩⇩⇩ Private ⇩⇩⇩
*/

void AeonTextBoxElement::refreshValues()
{
  if (dirtyTextValue) {
    applyTextValue();
    dirtyTextValue = false;
  }
  if (dirtyState) {
    applyState();
    alignText();
    dirtyState = false;
  }
  if (dirtyStyle) {
    applyStyle();
    dirtyStyle = false;
  }
  if (dirtyBounds) {
    applyBounds();
    dirtyBounds = false;
  }
}

void AeonTextBoxElement::applyBounds()
{
  shape.position(bounds.position());
  shape.size(bounds.size());
  alignText();
}

void AeonTextBoxElement::applyState()
{
  applyStyle();
  applyStateStyle(AeonElementState::HOVER);
  applyStateStyle(AeonElementState::FOCUS);
  applyStateStyle(AeonElementState::CLICKED);
}

void AeonTextBoxElement::applyStyle()
{
  applyStyle(defaultStyle);
}

void AeonTextBoxElement::applyStyle(const AeonStyleSheet& style)
{
  if (style.radius.has_value()) shape.radius(style.radius.value());
  if (style.borderSize.has_value()) shape.borderSize(style.borderSize.value());
  if (style.borderColor.has_value()) shape.borderColor(style.borderColor.value());
  if (style.fillColor.has_value()) shape.fillColor(style.fillColor.value());
  if (style.textColor.has_value()) {
    const sf::Color& textColor = style.textColor.value().getSfColor();
    text.setFillColor(textColor);
    text.setOutlineColor(textColor);
    text.setOutlineThickness(0.1);
    cursorShape.setFillColor(textColor);
  }
  if (style.fontSize.has_value()) {
    fontSize = style.fontSize.value();
    text.setCharacterSize(fontSize);
  }
  if (style.fontName.has_value()) {
    textFont = Eng::Fonts::Instance().getFont(style.fontName.value());
    if (textFont) {
      text.setFont(*textFont);
    }
  }
}

void AeonTextBoxElement::applyStateStyle(AeonElementState state)
{
  if (!hasState(state)) {
    return;
  }
  applyStyle(getStateStyle(state));
}

void AeonTextBoxElement::applyTextValue()
{
  text.setString(valueString);
  cursorIndex = valueString.getSize();
  alignText();
}

const Vector2f AeonTextBoxElement::getCurrentPadding()
{
  Vector2f padding = defaultStyle.padding.value_or(Vector2f(0, 0));
  if (hasState(AeonElementState::HOVER)) {
    const AeonStyleSheet& style = getStateStyle(AeonElementState::HOVER);
    if (style.padding.has_value()) {
      padding = style.padding.value();
    }
  }
  return padding;
}

void AeonTextBoxElement::alignText()
{
  if (!textFont) return;

  text.setCharacterSize(fontSize);

  float leading = textFont->getLineSpacing(fontSize);
  const sf::FloatRect& globalBounds = text.getGlobalBounds();
  const sf::FloatRect& localBounds = text.getLocalBounds();

  const Vector2f padding = getCurrentPadding();

  Vector2f textOrigin(
    -padding.x,
    -(bounds.height() - leading - localBounds.top) / 2
  );

  text.setOrigin(textOrigin);
  text.setPosition(bounds.position());

  cursorShape.setSize(sf::Vector2f(1, leading + 2));
  cursorShape.setOrigin(textOrigin);
  alignCursor();
}

void AeonTextBoxElement::alignCursor()
{
  sf::Vector2f cursorPosition(bounds.position());
  bool isPassword = false;

  if (valueString.getSize() > 0) {
    if (!isPassword && textFont) {
      sf::String befStr = valueString.substring(0, cursorIndex);
      sf::Text befText(befStr, *textFont);
      befText.setCharacterSize(fontSize);
      cursorPosition.x = bounds.x() + befText.getGlobalBounds().width + 1;
    } else {
      int index = std::max<int>(0, cursorIndex - 1);
      cursorPosition.x = text.findCharacterPos(index).x;
    }
  }

  cursorPosition.y -= 1;
  cursorShape.setPosition(cursorPosition);
}

}  // namespace ae