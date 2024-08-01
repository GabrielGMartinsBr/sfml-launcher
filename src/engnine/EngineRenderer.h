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
#include "engnine/OnRender.h"
#include "engnine/OnUpdate.h"
#include "engnine/Viewport.hpp"

namespace Eng {

struct EngineRenderer {
  EngineRenderer(unsigned int _width, unsigned int _height);

  void addViewport(SharedPtr<Eng::Viewport> vp);

  // Update

  void addToUpdateList(Eng::OnUpdate* instance);

  void removeFromUpdateList(Eng::OnUpdate* instance);

  // Render

  void addToRenderList(Eng::OnRender* instance);

  void removeFromRenderList(Eng::OnRender* instance);

  void markZOrderDirty();

  void render(sf::RenderTarget* target);

  sf::RenderTexture& getRenderTexture();

  /**
   *
   *  Private starts here
   *
   */

 private:
  static bool compareZ(const OnRender* a, const OnRender* b);

  unsigned int width;
  unsigned int height;
  bool zDirty;

  sf::RenderWindow* window;
  Vector<SharedPtr<Eng::Viewport>> viewports;
  SharedPtr<Eng::Viewport> defaultVp;

  Vector<Eng::OnUpdate*> updateList;
  Vector<Eng::OnRender*> renderList;

  sf::Sprite bufferSprite;
  sf::RenderTexture renderTexture;

  sf::Sprite fpsSprite;
  sf::Font font;
  sf::Text fpsText;

  void createBuffer();

  void createFpsText();

  void createDefaultViewport();

  void clearViewports();

  void update();

  void render();

  void renderViewports();

  void renderViewport(Eng::Viewport& vp);

  void renderBuffer(sf::RenderTarget* target);

  void renderFps();

  void sortZ();
};

}  // namespace Eng