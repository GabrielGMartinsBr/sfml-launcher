#include "./AeonTextBoxElement.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <unordered_set>

#include "aeon/enums/AeonElementState.h"
#include "aeon/toolkit/ElementBounds.h"
#include "aeon/window/AeonElement.h"
#include "aeon/window/AeonStyleSheet.h"
#include "aeon/window/AeonWindowManager.h"
#include "engnine/base/Fonts.h"

namespace ae {

static const int cursorBlinkTime = 500;

AeonTextBoxElement::AeonTextBoxElement() :
    textFont(nullptr),
    fontSize(16),
    valueString(""),
    cursorIndex(0),
    showCursor(false),
    isPassword(false),
    dirtyTextValue(false)
{
  setStyle(textBoxDefaultStyle);
  applyStyle();
  applyBounds();
}

/*
  Draw methods
*/

void AeonTextBoxElement::drawShapesTo(RenderTarget& target)
{
  refreshValues();
  shape.drawTo(target);
}

void AeonTextBoxElement::drawContentsTo(RenderTarget& target)
{
  target.draw(text);
  if (hasFocus && showCursor) {
    target.draw(cursorShape);
  }
}

/*
  Aeon event handlers
*/

void AeonTextBoxElement::handleAeonUpdate(ULong ts)
{
  AeonElement::handleAeonUpdate(ts);
  if (ts - lastCursorBlinkTs > cursorBlinkTime) {
    showCursor = !showCursor;
    lastCursorBlinkTs = ts;
  }
}

void AeonTextBoxElement::handleKeyPressed(const AeKeyEvent& event)
{
  switch (event.code) {
    case sf::Keyboard::Left:
      handleArrowLeftPressed(event.control);
      break;
    case sf::Keyboard::Right:
      handleArrowRightPressed(event.control);
      break;
    case sf::Keyboard::BackSpace:
      handleBackspacePressed(event.control);
      break;
    default:
      break;
  }
}

void AeonTextBoxElement::handleTextEntered(const AeTextEvent& event)
{
  static const std::unordered_set<char32_t> ignoredCharacters = { '\t', '\b', 13 };
  if (ignoredCharacters.count(event.unicode) > 0) {
    return;
  }
  handleInput(event.unicode);
}

/*

*/

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
  cursorIndex = valueString.getSize();
  dirtyTextValue = true;
  return valueString;
}

void AeonTextBoxElement::setFocus(bool value)
{
  AeonElement::setFocus(value);
  if (hasFocus) {
    revealCursor();
  }
}

void AeonTextBoxElement::setIsPassword(bool value)
{
  isPassword = true;
  dirtyTextValue = true;
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
  applyStateStyle(AeonElementState::FOCUS);
  applyStateStyle(AeonElementState::HOVER);
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
  text.setString(isPassword ? getPasswordText() : valueString);
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
  float border = defaultStyle.borderSize.value_or(0);
  float totalBorder = border * 2;

  float height = bounds.height() - totalBorder;

  Vector2f textOrigin(
    -padding.x,
    -(height - leading) / 2 + border
  );

  if (isPassword && !valueString.isEmpty()) {
    textOrigin.y = -(height - globalBounds.height) / 2 + localBounds.top;
  }

  text.setOrigin(textOrigin);
  text.setPosition(0, 0);

  Vector2f cursorSize(1, leading + 2);
  cursorShape.setSize(cursorSize);
  sf::Vector2f cursorOrigin(-padding.x, -(height - cursorSize.y) / 2);
  cursorShape.setOrigin(cursorOrigin);
  alignCursor();
}

void AeonTextBoxElement::alignCursor()
{
  sf::Vector2f cursorPosition(0, 0);

  if (valueString.getSize() > 0) {
    if (!isPassword && textFont) {
      sf::String befStr = valueString.substring(0, cursorIndex);
      sf::Text befText(befStr, *textFont);
      befText.setCharacterSize(fontSize);
      cursorPosition.x = befText.getGlobalBounds().width + 1;
    } else {
      int index = std::max<int>(-1, cursorIndex - 1);
      if (index > -1) {
        cursorPosition.x = text.findCharacterPos(index).x;
      } else {
        cursorPosition.x = 0;
      }
    }
  }

  cursorShape.setPosition(cursorPosition);
}

void AeonTextBoxElement::revealCursor()
{
  lastCursorBlinkTs = AeonWindowManager::Instance().getTimestamp();
  showCursor = true;
}

/*
    Text input event handlers
*/

void AeonTextBoxElement::handleArrowLeftPressed(bool isCtrlPressed)
{
  moveCursorLeft(isCtrlPressed);
}

void AeonTextBoxElement::handleArrowRightPressed(bool isCtrlPressed)
{
  moveCursorRight(isCtrlPressed);
}

void AeonTextBoxElement::handleBackspacePressed(bool isCtrlPressed)
{
  if (valueString.isEmpty() || cursorIndex < 1) {
    return;
  }
  if (isCtrlPressed) {
    valueString.erase(0, cursorIndex);
    cursorIndex = 0;
  } else {
    valueString.erase(cursorIndex - 1, 1);
    moveCursorLeft();
  }
  applyTextValue();
  alignCursor();
  revealCursor();
}

void AeonTextBoxElement::handleInput(sf::Uint32 unicode)
{
  if (unicode < 128 || unicode >= 32) {
    valueString.insert(cursorIndex, unicode);
    applyTextValue();
    moveCursorRight();
    // revealCursor();
  }
}

/*
    Text input methods
*/

void AeonTextBoxElement::moveCursorLeft(bool isCtrlPressed)
{
  if (cursorIndex == 0) {
    return;
  }
  if (isCtrlPressed) {
    cursorIndex = 0;
  } else {
    cursorIndex--;
  }
  alignCursor();
  revealCursor();
}

void AeonTextBoxElement::moveCursorRight(bool isCtrlPressed)
{
  if (cursorIndex >= valueString.getSize()) {
    return;
  }
  if (isCtrlPressed) {
    cursorIndex = valueString.getSize();
  } else {
    cursorIndex++;
  }
  alignCursor();
  revealCursor();
}

/*
  Utils
*/

sf::String AeonTextBoxElement::getPasswordText()
{
  sf::String passText = "";
  for (int i = 0; i < valueString.getSize(); i++) {
    passText += "*";
  }
  return passText;
}

}  // namespace ae