#include "TilemapLayer.h"

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <algorithm>

#include "Log.hpp"
#include "engnine/Engine.h"

namespace Eng {

TilemapLayer::TilemapLayer(int width, int height, int y, int priority, int layersN, int oy) :
    y(y),
    priority(priority),
    layersN(layersN)
{
  isDisposed = false;
  addedToEngineCycles = false;

  create(width, height);
  calcZ();

  addToEngineCycles();
}

TilemapLayer::~TilemapLayer()
{
  removeFromEngineCycles();
}

void TilemapLayer::create(int width, int height)
{
  rendTex.create(width, height);
}

void TilemapLayer::calcZ()
{
  if (priority == 0) {
    z = 0;
  } else if (y < 15) {
    z = (y + priority + 1) * 32;
  } else {
    z = (15 + priority) * 32;
  }
  Engine::getInstance().markZOrderDirty();
}

void TilemapLayer::update(int oy)
{
  if (y < oy) {
    return;
  }
  // nextZ = ((y - oy) + priority + 1) * 32;
  nextZ = ((y - oy + priority) % layersN + 1) * 32;
  if (nextZ != z) {
    z = nextZ;
    Engine::getInstance().markZOrderDirty();
    Log::out() << "z: " << z << " y: " << y << " oy: " << oy << " priority: " << priority;
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