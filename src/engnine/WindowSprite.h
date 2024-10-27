#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "aeon/toolkit/ElementBounds.h"
#include "engnine/IOnRender.h"
#include "engnine/Viewport.hpp"

namespace Eng {

using ae::ElementBounds;

struct WindowSprite : IOnRender {
  ElementBounds& bounds;
  sf::View& view;
  sf::Sprite sprite;

  int visible;

  WindowSprite(ElementBounds& bounds, sf::View& view, Viewport* viewport);

  ~WindowSprite();

  void onRender(sf::RenderTexture& renderTexture) override;

  bool shouldRender() const override;

  int getZIndex() const override;

  void dispose();

  sf::Sprite& getSprite();

  void setZ(int v);

  void setOpacity(int v);

 private:
  Viewport* viewport;
  int z;
  int opacity;

  bool isDisposed;
  bool addedToEngineCycles;

  sf::Color color;

  void addToEngineCycles();

  void removeFromEngineCycles();
};

}  // namespace Eng