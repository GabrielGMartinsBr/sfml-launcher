#include "engnine/GraphicsRenderer.h"

#include <SFML/Graphics/Font.hpp>

#include "engnine/IOnUpdate.h"
#include "engnine/Lists.hpp"
#include "engnine/Timer.hpp"
#include "engnine/base/Fonts.h"

namespace Eng {

GraphicsRenderer::GraphicsRenderer(sf::RenderTexture& renderTexture) :
    lists(Lists::Instance()),
    renderTexture(renderTexture)
{
  createFpsSprite();
}

void GraphicsRenderer::render(sf::RenderTarget& target)
{
  clearRenderTexture();
  updateSprites();
  renderSprites();
  setRenderSpriteTexture();
  target.draw(renderSprite);
}

/*
  ⇩⇩⇩ Private ⇩⇩⇩
*/

void GraphicsRenderer::createFpsSprite()
{
  const sf::Font* result = Fonts::Instance().getFont("Arial");
  assert(result);
  font = *result;
  fpsSprite.setFont(font);
  fpsSprite.setFillColor(sf::Color::White);
  fpsSprite.setOutlineColor(sf::Color::Black);
  fpsSprite.setOutlineThickness(1.5);
  fpsSprite.setCharacterSize(14);
  fpsSprite.setPosition(4, 458);
}

void GraphicsRenderer::clearRenderTexture()
{
  renderTexture.clear();
  clearViewports();
}

void GraphicsRenderer::clearViewports()
{
  for (Viewport* viewport : lists.viewports) {
    if (!viewport->shouldRender()) {
      continue;
    }
    viewport->clear();
  }
}

void GraphicsRenderer::updateSprites()
{
  for (IOnUpdate* updateInst : lists.updateList) {
    updateInst->onUpdate();
  }
}

void GraphicsRenderer::renderSprites()
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

  renderFpsSprite();
}

void GraphicsRenderer::renderFpsSprite()
{
  fpsSprite.setString(std::to_string(Timer::getInstance().getFps()));
  renderTexture.draw(fpsSprite);
}

void GraphicsRenderer::setRenderSpriteTexture()
{
  renderTexture.display();
  renderSprite.setTexture(renderTexture.getTexture());
}

}  // namespace Eng