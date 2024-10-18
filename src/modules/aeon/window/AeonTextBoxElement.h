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

  void handleAeonUpdate(ULong ts);
  void handleKeyPressed(const AeKeyEvent& event);
  void handleTextEntered(const AeTextEvent& event);

  void drawTo(RenderTarget& target) override;

  void flush();

  const sf::String& getValue();
  const sf::String& setValue(const sf::String& value);

  void setFocus(bool value);

 private:
  RoundedRectShape shape;
  RectangleShape cursorShape;
  const sf::Font* textFont;
  float fontSize;
  sf::String valueString;
  sf::Text text;
  size_t cursorIndex;
  bool showCursor;
  ULong lastCursorBlinkTs;
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
  void handleBackspace(bool isCtrlPressed);
  void handleInput(sf::Uint32 unicode);


  void moveCursorLeft(bool isCtrlPressed = false);
  void moveCursorRight(bool isCtrlPressed = false);
};

}  // namespace ae