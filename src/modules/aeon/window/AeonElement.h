#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <cstdint>

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

  AeonElement(const ElementBounds& bounds);
  AeonElement(const ElementBounds& bounds, const AeonStyleSheet& defaultStyle);

  inline virtual AeonElementType getType() const = 0;

  inline bool intersects(float x, float y) const
  {
    return bounds.intersects(x, y, defaultStyle.borderSize.value_or(0));
  }

  virtual void drawTo(RenderTarget& target);

  const ElementBounds& getBounds();
  void setBounds(const ElementBounds& value);

  void setPosition(float x, float y);
  void setSize(float width, float height);

  float x();
  float y();

  void x(float value);
  void y(float value);

  void addState(AeonElementState state);
  void removeState(AeonElementState state);
  bool hasState(AeonElementState state) const;
  void clearState();

  const AeonPartialStyleSheet& getStyle();
  void setStyle(const AeonPartialStyleSheet& style);

  const AeonPartialStyleSheet& getStateStyle(AeonElementState state);
  void setStateStyle(AeonElementState state, const AeonPartialStyleSheet& style);

  AeonPartialStyleSheet& getMutableStyle();
  AeonPartialStyleSheet& getMutableStyle(AeonElementState state);
  AeonPartialStyleSheet& getMutableStyle(const String& stateName);

  bool isFocusable() const;
  void setFocusable(bool value);


 protected:
  ElementBounds bounds;
  uint8_t states;
  AeonPartialStyleSheet defaultStyle;
  UnMap<AeonElementState, AeonPartialStyleSheet> stateStyles;
  bool dirtyBounds;
  bool dirtyState;
  bool dirtyStyle;
  bool focusable;
};

}  // namespace ae