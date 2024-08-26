#include "EngineRenderer.h"

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

#include "engnine/IOnRender.h"
#include "engnine/Lists.hpp"
#include "engnine/IOnUpdate.h"
#include "engnine/Timer.hpp"
#include "engnine/Viewport.hpp"

namespace Eng {

EngineRenderer::EngineRenderer(
  unsigned int _width, unsigned int _height
) :
    lists(Lists::Instance()),
    width(_width),
    height(_height)
{
  createBuffer();
}

void EngineRenderer::render(sf::RenderTarget* target)
{
  update();
  renderTexture.clear();
  clearViewports();
  render();
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
    throw std::runtime_error("Could not load font.");
  }
  fpsText.setFont(font);
  fpsText.setFillColor(sf::Color::White);
  fpsText.setOutlineColor(sf::Color::Black);
  fpsText.setOutlineThickness(1.5);
  fpsText.setCharacterSize(14);
  fpsText.setPosition(4, 458);
}

void EngineRenderer::clearViewports()
{
  for (Viewport* viewport : lists.viewports) {
    if (!viewport->shouldRender()) {
      continue;
    }
    viewport->clear();
  }
}

void EngineRenderer::update()
{
  for (Eng::IOnUpdate* updateInst : lists.updateList) {
    updateInst->onUpdate();
  }
}

void EngineRenderer::render()
{
  lists.sortZ();

  for (Eng::Viewport* viewport : lists.viewports) {
    if (!viewport->shouldRender()) {
      continue;
    }
    for (Eng::IOnRender* renderInst : viewport->children) {
      if (!renderInst->shouldRender()) {
        continue;
      }
      renderInst->onRender(viewport->rd);
    }
  }

  for (Eng::IOnRender* renderInst : lists.renderList) {
    if (!renderInst->shouldRender()) {
      continue;
    }
    renderInst->onRender(renderTexture);
  }

  renderFps();
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

}  // namespace Eng