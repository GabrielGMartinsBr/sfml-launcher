#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "engnine/Bitmap.h"
#include "engnine/EngineRenderEntity.hpp"
#include "engnine/Viewport.hpp"

namespace Eng {

struct WindowFrame : EngineRenderEntity {
  sf::Sprite backSprite;
  sf::Sprite borderSprite;
  sf::Texture texture;
  sf::RenderTexture rendTex;

  int width;
  int height;

  bool visible;

  WindowFrame(const sf::View& view, Viewport* viewport);

  void onRender(sf::RenderTexture& renderTexture) override;

  bool shouldRender() const override;

  int getZIndex() const override;

  void setZ(int value);

  void setOpacity(uint8_t value);
  void setBackOpacity(uint8_t value);

  void update(Bitmap* windowSkin);

  void dispose();

 private:
  const sf::View& view;
  Viewport* viewport;
  int z;
  uint8_t opacity;
  uint8_t backOpacity;

  sf::Color backColor;
  sf::Color borderColor;

  bool isDisposed;

  sf::Texture backTexture;

  void updateBackgroundSprite(Bitmap* windowSkin);

  void updateBorder(Bitmap* windowSkin);
};

}  // namespace Eng