#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "engnine/Lists.hpp"

namespace Eng {

struct GraphicsRenderer {
  sf::Font font;
  sf::Text fpsSprite;

  Lists& lists;

  sf::RenderTexture& renderTexture;
  sf::Sprite renderSprite;

  GraphicsRenderer(sf::RenderTexture& renderTexture);

  void render(sf::RenderTarget& target);

 private:

  void createFpsSprite();

  void clearRenderTexture();

  void clearViewports();

  void updateSprites();

  void renderSprites();

  void renderFpsSprite();

  void setRenderSpriteTexture();
};

}  // namespace Eng