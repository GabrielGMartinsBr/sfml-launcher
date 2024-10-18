#pragma once

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

#include "aeon/base/AeonIntegrable.h"
#include "aeon/toolkit/RoundedRectShape.h"
#include "aeon/window/AeonElement.h"

namespace ae {

using sf::Vector2f;

class AeonTextBoxElement : public AeonElement, public AeonIntegrable {
 public:

  AeonTextBoxElement();

  inline AeonElementType getType() const override
  {
    return AeonElementType::TEXT_BOX;
  }

  void drawTo(RenderTarget& target) override;

  void flush();

 private:
  RoundedRectShape shape;
  sf::Text text;
  const sf::Font* textFont;
  float fontSize;

  void refreshValues();
  void applyBounds();
  void applyState();
  void applyStyle();
  void applyStyle(const AeonStyleSheet& style);
  void applyStateStyle(AeonElementState state);

  void alignText();

  const Vector2f getCurrentPadding();
};

}  // namespace ae