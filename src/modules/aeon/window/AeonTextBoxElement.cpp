#include "./AeonTextBoxElement.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
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
    AeonElement(ElementBounds(), textBoxDefaultStyle),
    textFont(nullptr),
    fontSize(16),
    valueString(""),
    cursorIndex(0),
    showCursor(false),
    isPassword(false),
    dirtyTextValue(false)
{
  applyStyle();
  applyBounds();
}

void AeonTextBoxElement::handleAeonUpdate(ULong ts)
{
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

void AeonTextBoxElement::drawTo(RenderTarget& target)
{
  refreshValues();
  shape.drawTo(target);
  target.draw(text);
  if (hasFocusValue && showCursor) {
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
  cursorIndex = valueString.getSize();
  dirtyTextValue = true;
  return valueString;
}

void AeonTextBoxElement::setFocus(bool value)
{
  AeonElement::setFocus(value);
  if (hasFocusValue) {
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

  Vector2f textOrigin(
    -padding.x,
    -(bounds.height() - leading - localBounds.top) / 2
  );

  if (isPassword && !valueString.isEmpty()) {
    textOrigin.y = -(bounds.height() - globalBounds.height) / 2 + localBounds.top;
  }

  text.setOrigin(textOrigin);
  text.setPosition(bounds.position());

  cursorShape.setSize(sf::Vector2f(1, leading + 2));
  cursorShape.setOrigin(textOrigin);
  alignCursor();
}

void AeonTextBoxElement::alignCursor()
{
  sf::Vector2f cursorPosition(bounds.position());

  if (valueString.getSize() > 0) {
    if (!isPassword && textFont) {
      sf::String befStr = valueString.substring(0, cursorIndex);
      sf::Text befText(befStr, *textFont);
      befText.setCharacterSize(fontSize);
      cursorPosition.x = bounds.x() + befText.getGlobalBounds().width + 1;
    } else {
      int index = std::max<int>(-1, cursorIndex - 1);
      if (index > -1) {
        cursorPosition.x = text.findCharacterPos(index).x;
      } else {
        cursorPosition.x = bounds.x();
      }
    }
  }

  cursorPosition.y -= 1;
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