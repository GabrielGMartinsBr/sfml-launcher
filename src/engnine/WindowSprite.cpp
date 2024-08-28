#include "engnine/WindowSprite.h"

#include "engnine/Lists.hpp"

namespace Eng {

/*
  ⇩⇩⇩ Public ⇩⇩⇩
*/

WindowSprite::WindowSprite(Viewport* viewport) :
    viewport(viewport),
    color(255, 255, 255, 255)
{
  z = 0;
  opacity = 255;
  visible = true;
  addedToEngineCycles = false;
  isDisposed = false;
  addToEngineCycles();
}

WindowSprite::~WindowSprite()
{
  removeFromEngineCycles();
}

void WindowSprite::onRender(sf::RenderTexture& renderTexture)
{
  renderTexture.draw(sprite);
}

bool WindowSprite::shouldRender() const
{
  return !isDisposed && visible;
}

int WindowSprite::getZIndex() const
{
  return z;
}

void WindowSprite::dispose()
{
  isDisposed = true;
  removeFromEngineCycles();
}

void WindowSprite::setZ(int v)
{
  z = v;
  Lists::Instance().markZOrderDirty();
}

void WindowSprite::setOpacity(int v)
{
  opacity = v;
  color.a = opacity;
  sprite.setColor(color);
}

/*
  ⇩⇩⇩ Private ⇩⇩⇩
*/

void WindowSprite::addToEngineCycles()
{
  if (addedToEngineCycles) {
    return;
  }
  Lists::Instance().addRenderEntry(this);
  addedToEngineCycles = true;
}

void WindowSprite::removeFromEngineCycles()
{
  if (!addedToEngineCycles) {
    return;
  }
  Lists::Instance().removeRenderEntry(this);
  addedToEngineCycles = false;
}

}  // namespace Eng