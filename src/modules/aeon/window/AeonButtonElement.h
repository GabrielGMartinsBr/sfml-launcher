#pragma once

#include <SFML/System/String.hpp>

#include "aeon/base/AeonIntegrable.h"
#include "aeon/enums/AeonElementState.h"
#include "aeon/toolkit/ElementBounds.h"
#include "aeon/toolkit/RoundedRectShape.h"
#include "aeon/window/AeonElement.h"

namespace ae {

using sf::RenderTarget;

class AeonButtonElement : public AeonElement, public AeonIntegrable {
 public:
  sf::String text;

  AeonButtonElement(const ElementBounds& bounds);

  inline AeonElementType getType() const override
  {
    return AeonElementType::BUTTON;
  }

  void drawTo(RenderTarget& target) override;

  const sf::String& setText(const sf::String& value);

 private:
  RoundedRectShape shape;

  void refreshValues();
  void applyBounds();
  void applyState();
  void applyStyle();
  void applyStyle(const AeonPartialStyleSheet& style);
  void applyStateStyle(AeonElementState state);
};

}  // namespace ae