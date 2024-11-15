#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>

#include "aeon/base/AeonIntegrable.h"
#include "aeon/enums/AeonElementState.h"
#include "aeon/toolkit/RoundedRectShape.h"
#include "aeon/window/AeonElement.h"

namespace ae {

using sf::RenderTarget;

class AeonButtonElement : public AeonElement, public AeonIntegrable {
 public:
  sf::String textString;

  AeonButtonElement();

  inline AeonElementType getType() const override
  {
    return AeonElementType::BUTTON;
  }

  void drawShapesTo(RenderTarget& target) override;

  const sf::String& setText(const sf::String& value);

  void flush();

 private:
  RoundedRectShape shape;
  const sf::Font* textFont;
  float fontSize;
  sf::Text text;

  void refreshValues();
  void applyBounds();
  void applyState();
  void applyStyle();
  void applyStyle(const AeonStyleSheet& style);
  void applyStateStyle(AeonElementState state);

  void alignText();
  void adaptToTextSize();
};

}  // namespace ae