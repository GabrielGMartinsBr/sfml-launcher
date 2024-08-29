#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "engnine/Lists.hpp"

namespace Eng {

struct GraphicsRenderer {
  sf::Font font;
  sf::Text fpsSprite;

  Lists& lists;

  sf::RenderTexture& renderTexture;
  sf::Sprite renderSprite;
  sf::Sprite frozenSprite;

  GraphicsRenderer(sf::RenderWindow& target, sf::RenderTexture& renderTexture);

  void render();

  void freeze();

  void transition(float progress);

  void transitionEnd();

 private:
  sf::RenderWindow& window;
  bool frozen;
  sf::Color transitionColor;
  sf::Texture frozenTexture;

  void createFpsSprite();

  void clearRenderTexture();

  void clearViewports();

  void updateSprites();

  void renderSprites();

  void renderFpsSprite();

  void setRenderSpriteTexture();
};

}  // namespace Eng