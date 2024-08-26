#pragma once

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

#include "engnine/Lists.hpp"

namespace Eng {

struct EngineRenderer {
  EngineRenderer(unsigned int _width, unsigned int _height);

  void render(sf::RenderTarget* target);

  sf::RenderTexture& getRenderTexture();

  /**
   *
   *  Private starts here
   *
   */

 private:
  unsigned int width;
  unsigned int height;

  Lists& lists;
  sf::RenderWindow* window;

  sf::Sprite bufferSprite;
  sf::RenderTexture renderTexture;

  sf::Sprite fpsSprite;
  sf::Font font;
  sf::Text fpsText;

  void createBuffer();

  void createFpsText();

  void clearViewports();

  void update();

  void render();

  void renderBuffer(sf::RenderTarget* target);

  void renderFps();
};

}  // namespace Eng