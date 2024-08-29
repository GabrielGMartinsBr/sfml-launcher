#include "engnine/GraphicsRenderer.h"

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Window.hpp>

#include "engnine/IOnUpdate.h"
#include "engnine/Lists.hpp"
#include "engnine/Timer.hpp"
#include "engnine/base/Fonts.h"

namespace Eng {

GraphicsRenderer::GraphicsRenderer(sf::RenderWindow& window, sf::RenderTexture& renderTexture) :
    lists(Lists::Instance()),
    window(window),
    renderTexture(renderTexture),
    transitionColor(255, 255, 255, 255)
{
  frozen = false;
  createFpsSprite();
}

void GraphicsRenderer::render()
{
  clearRenderTexture();
  updateSprites();
  renderSprites();
  setRenderSpriteTexture();

  window.clear(sf::Color::Transparent);
  if (frozen) {
    window.draw(frozenSprite);
    window.draw(renderSprite);
  } else {
    window.draw(renderSprite);
  }
  window.display();
}

void GraphicsRenderer::freeze()
{
  clearRenderTexture();
  renderSprites();
  renderTexture.display();
  frozenTexture = renderTexture.getTexture();
  frozenSprite.setTexture(frozenTexture);
  frozen = true;
}

void GraphicsRenderer::transition(float progress)
{
  transitionColor.a = 255 * progress;
  renderSprite.setColor(transitionColor);
}

void GraphicsRenderer::transitionEnd()
{
  clearRenderTexture();
  updateSprites();
  renderSprites();
  setRenderSpriteTexture();
  frozen = false;
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
  renderTexture.clear(sf::Color::Transparent);
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