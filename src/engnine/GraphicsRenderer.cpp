#include "engnine/GraphicsRenderer.h"

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Window.hpp>

#include "Log.hpp"
#include "engnine/IOnUpdate.h"
#include "engnine/Lists.hpp"
#include "engnine/Performance.hpp"
#include "engnine/Shaders.h"
#include "engnine/Timer.hpp"
#include "engnine/base/Fonts.h"

namespace Eng {

Performance renderPer;

using sf::Texture;

GraphicsRenderer::GraphicsRenderer(ProjectWindow& projectWindow, sf::RenderTexture& renderTexture) :
    projectWindow(projectWindow),
    lists(Lists::Instance()),
    shaders(Shaders::Instance()),
    renderTexture(renderTexture)
{
  createFpsSprite();
}

void GraphicsRenderer::render()
{
  clearRenderTexture();
  updateSprites();
  renderSprites();
  setRenderSpriteTexture();

  projectWindow.window.clear(sf::Color::Transparent);
  projectWindow.window.draw(renderSprite);
  projectWindow.window.display();
}

void GraphicsRenderer::freeze()
{
  clearRenderTexture();
  renderSprites();
  renderTexture.display();
  frozenTexture = renderTexture.getTexture();
}

void GraphicsRenderer::renderFadeTransitionState(float progress)
{
  clearRenderTexture();
  updateSprites();
  renderSprites();
  setRenderSpriteTexture();

  projectWindow.window.clear(sf::Color::Transparent);
  shaders.fadeTransitionShader->setUniform("alpha", progress);
  shaders.fadeTransitionShader->setUniform("frozenTexture", frozenTexture);
  projectWindow.window.draw(renderSprite, shaders.fadeTransitionShader.get());
  projectWindow.window.display();
}

void GraphicsRenderer::renderImageTransitionState(float progress, const Texture& transTexture, float vague)
{
  clearRenderTexture();
  updateSprites();
  renderSprites();
  setRenderSpriteTexture();

  shaders.imageTransitionShader->setUniform("frozenTexture", frozenTexture);
  shaders.imageTransitionShader->setUniform("progress", progress);
  shaders.imageTransitionShader->setUniform("vague", vague);
  shaders.imageTransitionShader->setUniform("transitionTexture", transTexture);

  projectWindow.window.clear(sf::Color::Transparent);
  projectWindow.window.draw(renderSprite, shaders.imageTransitionShader.get());
  projectWindow.window.display();
}

void GraphicsRenderer::transitionEnd()
{
  clearRenderTexture();
  updateSprites();
  renderSprites();
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
  fpsSprite.setPosition(4, projectWindow.height() - 22);
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
  renderPer.start();

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

  renderPer.average();
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