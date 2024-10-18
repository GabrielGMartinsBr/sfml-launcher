#pragma once

#include <ruby.h>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "AppDefs.h"
#include "aeon/base/AeonEvents.h"
#include "aeon/toolkit/AeonHitbox.hpp"
#include "aeon/toolkit/RoundedRectShape.h"
#include "aeon/window/AeonElement.h"
#include "engnine/IOnRender.h"
#include "engnine/Viewport.hpp"
#include "engnine/Window.h"

namespace ae {

using app::SPtr;
using app::ULong;
using app::Vector;
using sf::RenderTarget;

class AeonWindow : public Eng::Window, Eng::IOnRender {
 public:
  AeonWindow(VALUE rbObj, Eng::Viewport* viewport = nullptr);

  ~AeonWindow();

  void handleAeonUpdate(ULong ts);
  void handleMouseMoved(const AeMouseMoveEvent& event);
  void handleMousePressed(const AeMouseButtonEvent& event);
  void handleMouseReleased(const AeMouseButtonEvent& event);

  void setIsHover(bool value);
  void setIsFocused(bool value);

  void onRender(sf::RenderTexture& renderTexture) override;
  bool shouldRender() const override;
  int getZIndex() const override;

  bool intersects(float x, float y);

  void setRingVisibility(bool value);

  VALUE setter_x(VALUE v);
  VALUE setter_y(VALUE v);

  VALUE setter_width(VALUE v);
  VALUE setter_height(VALUE v);

  void method_dispose();

  void addElement(AeonElement* element);
  void removeElement(AeonElement* element);

  void drawElements(sf::RenderTarget& target);

 private:
  Vector<AeonElement*> elements;
  AeonElement* focusedElement;
  ULong timestamp;
  AeonHitBox hitBox;
  RoundedRectShape ring;
  sf::RenderTexture aeContent;
  sf::Sprite aeContentSpr;
  bool isRingVisible;
  bool isHover;
  bool isFocused;
  bool addedToEngineCycles;

  void addToEngineCycles();
  void removeFromEngineCycles();

  void updateContentPosition();
  void updateContentDimension();

  void setFocused(AeonElement* element);
};

}  // namespace ae