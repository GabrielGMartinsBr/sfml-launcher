#pragma once

#include <ruby.h>

#include <SFML/Graphics/RenderTarget.hpp>

#include "aeon/toolkit/RoundedRectShape.h"
#include "engnine/IOnRender.h"
#include "engnine/Viewport.hpp"
#include "engnine/Window.h"

namespace ae {

using sf::RenderTarget;

class AeonWindow : public Eng::Window, Eng::IOnRender {
 public:
  AeonWindow(VALUE rbObj, Eng::Viewport* viewport = nullptr);

  ~AeonWindow();

  void onRender(sf::RenderTexture& renderTexture) override;
  bool shouldRender() const override;
  int getZIndex() const override;

  void aeonUpdate() { }

  VALUE setter_x(VALUE v);
  VALUE setter_y(VALUE v);

  VALUE setter_width(VALUE v);
  VALUE setter_height(VALUE v);


 private:
  RoundedRectShape ring;
  bool addedToEngineCycles;

  void addToEngineCycles();
  void removeFromEngineCycles();
};

}  // namespace ae