#pragma once

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

#include "base/Sugars.hpp"
#include "engnine/Drawable.hpp"
#include "engnine/Viewport.hpp"

namespace Eng {

struct EngineRenderer {
  EngineRenderer(unsigned int _width, unsigned int _height);

  void addViewport(SharedPtr<Eng::Viewport> vp);

  void addDrawable(Eng::Drawable* drawable);

  void removeDrawable(Eng::Drawable* drawable);

  void markZOrderDirty();

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
  bool zDirty;

  sf::RenderWindow* window;
  Vector<SharedPtr<Eng::Viewport>> viewports;
  Vector<Eng::Drawable*> drawables;
  SharedPtr<Eng::Viewport> defaultVp;

  sf::Sprite bufferSprite;
  sf::RenderTexture renderTexture;

  sf::Sprite fpsSprite;
  sf::Font font;
  sf::Text fpsText;

  void createBuffer();

  void createFpsText();

  void createDefaultViewport();

  void clearViewports();

  void renderDrawables();

  void renderViewports();

  void renderViewport(Eng::Viewport& vp);

  void renderBuffer(sf::RenderTarget* target);

  void renderFps();

  static bool compareZ(const Drawable* a, const Drawable* b);

  void sortZ();
};

}  // namespace Eng