#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "engnine/Lists.hpp"
#include "engnine/Shaders.h"
#include "launcher/ProjectWindow.h"

namespace Eng {

using sf::RenderTexture;
using sf::RenderWindow;
using sf::Texture;

struct GraphicsRenderer {
  GraphicsRenderer(ProjectWindow& projectWindow, RenderTexture& renderTexture);

  void render();

  void freeze();

  void renderFadeTransitionState(float progress);

  void renderImageTransitionState(float progress, const Texture& transitionTexture, float vague);

  void transitionEnd();

 private:
  Lists& lists;
  Shaders& shaders;
  ProjectWindow& projectWindow;

  RenderTexture& renderTexture;
  sf::Sprite renderSprite;
  Texture frozenTexture;

  sf::Font font;
  sf::Text fpsSprite;

  void createFpsSprite();

  void clearRenderTexture();

  void clearViewports();

  void updateSprites();

  void renderSprites();

  void renderFpsSprite();

  void setRenderSpriteTexture();
};

}  // namespace Eng