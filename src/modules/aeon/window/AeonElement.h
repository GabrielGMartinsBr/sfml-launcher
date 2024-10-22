#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <cstdint>

#include "AppDefs.h"
#include "aeon/enums/AeonElementState.h"
#include "aeon/enums/AeonElementType.h"
#include "aeon/toolkit/ElementBounds.h"
#include "aeon/window/AeonStyleSheet.h"

namespace ae {

using app::ULong;
using app::UnMap;
using sf::RenderTarget;

class AeonElement {
 public:

  AeonElement();

  inline virtual AeonElementType getType() const = 0;

  virtual void drawShapesTo(RenderTarget& target);
  virtual void drawContentsTo(RenderTarget& target);

  virtual void handleAeonUpdate(ULong ts);
  virtual void handleClick();
  virtual void handleClickRelease();

  inline bool intersects(float x, float y) const
  {
    return bounds.intersects(x, y, defaultStyle.borderSize.value_or(0));
  }

  const ElementBounds& getBounds() const;
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

  const AeonStyleSheet& getStyle() const;
  void setStyle(const AeonStyleSheet& style);

  const AeonStyleSheet& getStateStyle(AeonElementState state);
  void setStateStyle(AeonElementState state, const AeonStyleSheet& style);

  AeonStyleSheet& getMutableStyle();
  AeonStyleSheet& getMutableStyle(AeonElementState state);
  AeonStyleSheet& getMutableStyle(const String& stateName);

  bool isFocusable() const;
  void setFocusable(bool value);

  bool getHasFocus();
  void setFocus(bool value);

  bool isClicked();
  bool isTriggered();

 protected:
  ElementBounds bounds;
  uint8_t states;
  AeonStyleSheet defaultStyle;
  UnMap<AeonElementState, AeonStyleSheet> stateStyles;
  bool dirtyBounds;
  bool dirtyState;
  bool dirtyStyle;
  bool sizeUndefined;

  bool focusable;
  bool hasFocus;
  bool clicked;
  bool triggered;
};

}  // namespace ae