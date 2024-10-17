#pragma once

#include <SFML/Graphics/RenderTarget.hpp>

#include "AppDefs.h"
#include "aeon/enums/AeonElementState.h"
#include "aeon/enums/AeonElementType.h"
#include "aeon/toolkit/ElementBounds.h"
#include "aeon/window/AeonStyleSheet.h"

namespace ae {

using app::UnMap;
using sf::RenderTarget;

class AeonElement {
 public:

  AeonElement(const ElementBounds& bounds, const AeonStyleSheet& style);

  inline virtual AeonElementType getType() const = 0;

  inline bool intersects(float x, float y) const;

  virtual void drawTo(RenderTarget& target);

  const ElementBounds& getBounds();
  void setBounds(const ElementBounds& value);

  void setPosition(float x, float y);
  void setSize(float width, float height);

  float x();
  float y();

  void x(float value);
  void y(float value);

  const AeonStyleSheet& getStyle();
  void setStyle(const AeonPartialStyleSheet& style);

  const AeonPartialStyleSheet& getStateStyle(AeonElementState state);
  void setStateStyle(AeonElementState state, const AeonPartialStyleSheet& style);

  const AeonElementState& getState();
  void setState(const AeonElementState& value);

  bool isFocusable() const;
  void setFocusable(bool value);


 protected:
  ElementBounds bounds;
  AeonStyleSheet defaultStyle;
  UnMap<AeonElementState, AeonPartialStyleSheet> stateStyles;
  AeonElementState elementState;
  bool dirtyBounds;
  bool dirtyStyle;
  bool focusable;
};

}  // namespace ae