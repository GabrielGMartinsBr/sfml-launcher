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

#include "engnine/OnRender.h"
#include "engnine/OnUpdate.h"
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

// OnUpdate List

void EngineRenderer::addToUpdateList(Eng::OnUpdate* instance)
{
  updateList.push_back(instance);
}

void EngineRenderer::removeFromUpdateList(Eng::OnUpdate* instance)
{
  auto it = std::find(updateList.begin(), updateList.end(), instance);
  if (it != updateList.end()) {
    updateList.erase(it);
  }
}

// OnRender List

void EngineRenderer::addToRenderList(Eng::OnRender* instance)
{
  renderList.push_back(instance);
  zDirty = true;
}

void EngineRenderer::removeFromRenderList(Eng::OnRender* instance)
{
  auto it = std::find(renderList.begin(), renderList.end(), instance);
  if (it != renderList.end()) {
    renderList.erase(it);
  }
}

void EngineRenderer::markZOrderDirty()
{
  zDirty = true;
}

void EngineRenderer::render(sf::RenderTarget* target)
{
  update();
  renderTexture.clear();
  // clearViewports();
  render();
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
  fpsText.setCharacterSize(14);
  fpsText.setPosition(4, 458);
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

void EngineRenderer::update()
{
  for (Eng::OnUpdate* updateInst : updateList) {
    updateInst->onUpdate();
  }
}

void EngineRenderer::render()
{
  if (zDirty) {
    sortZ();
  }
  for (Eng::OnRender* renderInst : renderList) {
    if (!renderInst->shouldRender()) {
      continue;
    }
    renderInst->onRender(renderTexture);
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

bool EngineRenderer::compareZ(const OnRender* a, const OnRender* b)
{
  return a->getZIndex() < b->getZIndex();
}

void EngineRenderer::sortZ()
{
  std::sort(renderList.begin(), renderList.end(), compareZ);
  zDirty = false;
}

}  // namespace Eng