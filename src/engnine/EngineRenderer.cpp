#pragma once

#include "engnine/EngineRenderer.h"

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <stdexcept>
#include <string>

#include "base/Sugars.hpp"
#include "engnine/Drawable.hpp"
#include "engnine/Timer.hpp"
#include "engnine/Viewport.hpp"

namespace Eng {

EngineRenderer::EngineRenderer(
  unsigned int _width, unsigned int _height
) :
    width(_width),
    height(_height),
    zDirty(false)
{
  createBuffer();
  createDefaultViewport();
}

void EngineRenderer::addViewport(SharedPtr<Eng::Viewport> vp)
{
  viewports.push_back(vp);
}

void EngineRenderer::addDrawable(Eng::Drawable* drawable)
{
  drawables.push_back(drawable);
  zDirty = true;
  // Log::out() << "(add) Drawables size: " << drawables.size();
}

void EngineRenderer::removeDrawable(Eng::Drawable* drawable)
{
  auto it = std::find(drawables.begin(), drawables.end(), drawable);
  if (it != drawables.end()) {
    drawables.erase(it);
  }
  Log::out() << "(remove) Drawables size: " << drawables.size();
}

void EngineRenderer::markZOrderDirty()
{
  zDirty = true;
}

void EngineRenderer::render(sf::RenderTarget* target)
{
  // renderTexture.clear(sf::Color::Transparent);
  renderTexture.clear();
  // clearViewports();
  renderDrawables();
  // renderViewports();
  renderBuffer(target);
}

sf::RenderTexture& EngineRenderer::getRenderTexture()
{
  return renderTexture;
}

/**
 *
 *  Private starts here
 *
 */

void EngineRenderer::createBuffer()
{
  sf::ContextSettings settings;
  // settings.antialiasingLevel = 0;
  renderTexture.create(width, height, settings);
  renderTexture.clear(sf::Color::Transparent);
  createFpsText();
}

void EngineRenderer::createFpsText()
{
  bool result = font.loadFromFile(
    "/run/media/home/common/gabrielmartins.dev/dev/cpp/orm-xp/sfml-launcher/assets/arial.ttf"
  );
  if (!result) {
    std::runtime_error("Could not load font.");
  }
  fpsText.setFont(font);
  fpsText.setFillColor(sf::Color::White);
  fpsText.setOutlineColor(sf::Color::Black);
  fpsText.setOutlineThickness(1.5);
  fpsText.setCharacterSize(16);
  fpsText.setPosition(8, 8);
}

void EngineRenderer::createDefaultViewport()
{
  defaultVp = MakeSharedPtr<Eng::Viewport>(0, 0, width, height);
  viewports.push_back(defaultVp);
}

void EngineRenderer::clearViewports()
{
  for (auto& vp : viewports) {
    if (!vp) {
      continue;
    }
    vp->getRgssViewport().clear();
  }
}

void EngineRenderer::renderDrawables()
{
  if (zDirty) {
    sortZ();
  }
  for (Eng::Drawable* drawable : drawables) {
    if (!drawable->shouldRender()) {
      continue;
    }
    drawable->update();
    drawable->draw(renderTexture);
  }
  renderFps();
}

void EngineRenderer::renderViewports()
{
  for (SharedPtr<Eng::Viewport>& vp : viewports) {
    if (!vp) {
      continue;
    }
    renderViewport(*vp);
  }
}

void EngineRenderer::renderViewport(Eng::Viewport& vp)
{
  vp.getRgssViewport().renderIn(
    renderTexture, sf::BlendAlpha
  );
  vp.getRgssViewport().display();
}

void EngineRenderer::renderBuffer(sf::RenderTarget* target)
{
  renderTexture.display();
  bufferSprite.setTexture(renderTexture.getTexture());
  target->draw(bufferSprite);
}

void EngineRenderer::renderFps()
{
  fpsText.setString(std::to_string(Timer::getInstance().getFps()));
  renderTexture.draw(fpsText);
}

bool EngineRenderer::compareZ(const Drawable* a, const Drawable* b)
{
  return a->getZPosition() < b->getZPosition();
}

void EngineRenderer::sortZ()
{
  // Log::out() << "sort";
  std::sort(drawables.begin(), drawables.end(), compareZ);
  zDirty = false;
}

}  // namespace Eng