#include "TilemapLayer.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "engnine/Engine.h"

namespace Eng {

TilemapLayer::TilemapLayer(int width, int height, int y, int priority, int oy) :
    y(y),
    oy(oy),
    priority(priority)
{
  isDisposed = false;
  addedToEngineCycles = false;

  create(width, height);
  calcZ();
  update(oy);

  addToEngineCycles();
}

TilemapLayer::~TilemapLayer()
{
  removeFromEngineCycles();
  dispose();
}

void TilemapLayer::create(int width, int height)
{
  rendTex.create(width, height);
  rendTex.clear(sf::Color::Transparent);
}

void TilemapLayer::calcZ()
{
  if (priority == 0) {
    z = 0;
  } else {
    z = (y + priority + 1) * 32;
  }
  Engine::getInstance().markZOrderDirty();
}

void TilemapLayer::update(int oy)
{
  this->oy = oy;
  nextZ = (y - oy + priority + 1) * 32;
  if (nextZ != z) {
    z = nextZ;
    Engine::getInstance().markZOrderDirty();
  }
}

int TilemapLayer::getZIndex() const
{
  return z;
}

inline bool TilemapLayer::shouldRender() const
{
  return !isDisposed;
}

void TilemapLayer::onRender(sf::RenderTexture& renderTexture)
{
  renderTexture.draw(sprite);
}

void TilemapLayer::dispose()
{
  isDisposed = true;
  removeFromEngineCycles();
}

/*
  Private
*/

void TilemapLayer::addToEngineCycles()
{
  if (addedToEngineCycles) {
    return;
  }
  Engine::getInstance().addToRenderList(this);
  addedToEngineCycles = true;
}

void TilemapLayer::removeFromEngineCycles()
{
  if (!addedToEngineCycles) {
    return;
  }
  Engine::getInstance().removeFromRenderList(this);
  addedToEngineCycles = false;
}

}  // namespace Eng