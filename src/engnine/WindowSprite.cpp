#include "engnine/WindowSprite.h"

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "engnine/Lists.hpp"

namespace Eng {

/*
  ⇩⇩⇩ Public ⇩⇩⇩
*/

WindowSprite::WindowSprite(ElementBounds& bounds, sf::View& view, const sf::Sprite& backSprite, Viewport* viewport) :
    bounds(bounds),
    view(view),
    backSprite(backSprite),
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
  renderTexture.setView(view);
  renderTexture.draw(sprite, sf::BlendAlpha);
  renderTexture.display();
  renderTexture.setView(renderTexture.getDefaultView());
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