#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>

#include "aeon/base/AeonEvents.h"
#include "aeon/base/AeonIntegrable.h"
#include "aeon/toolkit/RoundedRectShape.h"
#include "aeon/window/AeonElement.h"

namespace ae {

using app::ULong;
using sf::RectangleShape;
using sf::Vector2f;

class AeonTextBoxElement : public AeonElement, public AeonIntegrable {
 public:

  AeonTextBoxElement();

  inline AeonElementType getType() const override
  {
    return AeonElementType::TEXT_BOX;
  }

  void drawShapesTo(RenderTarget& target) override;
  void drawContentsTo(RenderTarget& target) override;

  void handleAeonUpdate(ULong ts) override;
  void handleKeyPressed(const AeKeyEvent& event);
  void handleTextEntered(const AeTextEvent& event);

  void flush();

  void setFocus(bool value);
  void setIsPassword(bool value);
  void setPlaceholder(const sf::String& value);

  const sf::String& getValue();
  const sf::String& setValue(const sf::String& value);

  bool isCursorOnBegin();
  bool isCursorOnEnd();

 private:
  RoundedRectShape shape;
  RectangleShape cursorShape;
  const sf::Font* textFont;
  float fontSize;
  sf::String valueString;
  sf::String placeholderString;
  sf::Text text;
  sf::Text placeholderText;
  size_t cursorIndex;
  ULong lastCursorBlinkTs;
  bool showCursor;
  bool isPassword;
  bool dirtyTextValue;

  void refreshValues();
  void applyBounds();
  void applyState();
  void applyStyle();
  void applyStyle(const AeonStyleSheet& style);
  void applyStateStyle(AeonElementState state);
  void applyTextValue();

  const Vector2f getCurrentPadding();

  void alignText();
  void alignCursor();
  void revealCursor();

  void handleArrowLeftPressed(bool isCtrlPressed);
  void handleArrowRightPressed(bool isCtrlPressed);
  void handleBackspacePressed(bool isCtrlPressed);
  void handleInput(sf::Uint32 unicode);

  void moveCursorLeft(bool isCtrlPressed = false);
  void moveCursorRight(bool isCtrlPressed = false);

  sf::String getPasswordText();
};

}  // namespace ae