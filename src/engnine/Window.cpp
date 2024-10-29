
#include "Window.h"

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cmath>

#include "aeon/toolkit/ElementBounds.h"
#include "engnine/Bitmap.h"
#include "engnine/Engine.h"
#include "engnine/Lists.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Viewport.hpp"
#include "engnine/WindowCursor.hpp"

namespace Eng {

Window::Window(Viewport *viewport) :
    Window(Qnil, viewport)
{
}

Window::Window(VALUE rbObj, Viewport *viewport) :
    EngineBase(rbObj),
    AeonIntegrable(rbObj),
    viewport(viewport),
    bounds(0, 0, 0, 0),
    windowSkin(nullptr),
    contents(nullptr),
    cursor_rect(new Rect(0, 0, 0, 0)),
    frame(viewport),
    contentsSprite(bounds, view, frame.backSprite, viewport),
    cursor(view, viewport)
{
  contentsDirty = true;
  skinDirty = true;
  dimensionsDirty = false;
  opacityDirty = false;

  active = true;
  visible = true;
  pause = false;

  z = 0;
  ox = 0;
  oy = 0;
  opacity = 255;
  back_opacity = 255;
  contents_opacity = 255;
  isDisposed = false;
  addedToEngineCycles = false;

  if (hasRbObj()) {
    bindRubyVars();
  }

  addToEngineCycles();
}

Window::~Window()
{
  removeFromEngineCycles();
}

void Window::onUpdate()
{
  if (isDisposed || bounds.isEmpty()) {
    return;
  }
  if (dimensionsDirty) {
    frame.rendTex.create(bounds.width(), bounds.height());
    dimensionsDirty = false;
  }
  updateOpacity();
  if (skinDirty) {
    updateFrameSprites();
    updateCursorRect();
    skinDirty = false;
    cursor_rect->markAsClean();
  }
  if (cursor_rect->isDirty()) {
    updateCursorRect();
    cursor_rect->markAsClean();
  }
  if (contentsDirty || (contents && contents->dirty)) {
    updateContentsSprite();
    contentsDirty = false;
    if (contents) {
      contents->dirty = false;
    }
  }
  cursor.setVisible(visible);
  cursor.update();
}

/*
  Private
*/

void Window::addToEngineCycles()
{
  if (addedToEngineCycles) {
    return;
  }
  Lists::Instance().addUpdateEntry(this);
  addedToEngineCycles = true;
}

void Window::removeFromEngineCycles()
{
  if (!addedToEngineCycles) {
    return;
  }
  Lists::Instance().removeUpdateEntry(this);
  addedToEngineCycles = false;
}

void Window::updateFrameSprites()
{
  frame.visible = visible;
  frame.width = bounds.width();
  frame.height = bounds.height();

  frame.update(windowSkin);

  frame.backSprite.setPosition(bounds.position());
  frame.borderSprite.setPosition(bounds.position());

  cursor.setWindowSkin(windowSkin);
}

void Window::updateContentsSprite()
{
  if (contents == nullptr) {
    return;
  }
  contentsSprite.sprite.setTexture(contents->getTexture());
  contentsSprite.sprite.setPosition(16, 16);
  contentsSprite.sprite.setColor(sf::Color(255, 255, 255, contents_opacity));
}

void Window::updateCursorRect()
{
  cursor.setBounds(
    ae::ElementBounds(
      cursor_rect->x.get() + 16,
      cursor_rect->y.get() + 16,
      cursor_rect->width.get(),
      cursor_rect->height.get()
    )
  );
  cursor_rect->markAsClean();
}

void Window::updateWindowSpriteZ()
{
  frame.setZ(z);
  contentsSprite.setZ(z + 2);
  cursor.setZIndex(z + 1);
  Lists::Instance().markZOrderDirty();
}
void Window::updateWindowSprite()
{
  frame.visible = visible;
  contentsSprite.visible = visible;
}

void Window::updateOpacity()
{
  if (!opacityDirty) {
    return;
  }

  frame.setOpacity(back_opacity);
  contentsSprite.setOpacity(contents_opacity);

  opacityDirty = false;
}

void Window::updateViewBounds()
{
  const sf::Vector2i &dimensions = Engine::getInstance().getDimensions();
  ElementBounds vb = bounds;
  view.setSize(vb.size());
  view.setCenter(vb.width() / 2, vb.height() / 2);
  view.setViewport(sf::FloatRect(
    vb.x() / dimensions.x,
    vb.y() / dimensions.y,
    vb.width() / dimensions.x,
    vb.height() / dimensions.y
  ));
}

}  // namespace Eng