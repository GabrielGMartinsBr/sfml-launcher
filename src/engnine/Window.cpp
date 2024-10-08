
#include "Window.h"

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cmath>

#include "engnine/Bitmap.h"
#include "engnine/Lists.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Viewport.hpp"

namespace Eng {

Window::Window(Viewport *viewport) :
    Window(Qnil, viewport)
{
}

Window::Window(VALUE rbObj, Viewport *viewport) :
    EngineBase(rbObj),
    viewport(viewport),
    windowSkin(nullptr),
    contents(nullptr),
    cursor_rect(new Rect(0, 0, 0, 0)),
    frame(viewport),
    contentsSprite(viewport),
    cursorSprite(viewport)
{
  contentsDirty = true;
  skinDirty = true;
  dimensionsDirty = false;
  opacityDirty = false;

  active = true;
  visible = true;
  pause = false;

  x = 0;
  y = 0;
  width = 0;
  height = 0;
  z = 0;
  ox = 0;
  oy = 0;
  opacity = 255;
  back_opacity = 255;
  contents_opacity = 255;
  isDisposed = false;
  addedToEngineCycles = false;

  cursorAniAlphaId = 0;

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
  if (isDisposed || width < 1 || height < 1) {
    return;
  }
  if (dimensionsDirty) {
    frame.rendTex.create(width, height);
    contentsSprite.rendTex.create(width, height);
    cursorSprite.rendTex.create(width, height);
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
  cursorSprite.visible = visible;
  frame.width = width;
  frame.height = height;

  frame.update(windowSkin);

  frame.backSprite.setPosition(x, y);
  frame.borderSprite.setPosition(x, y);
}

void Window::updateContentsSprite()
{
  if (contents == nullptr) {
    return;
  }
  contentsSprite.texture = contents->getTexture();
  contentsSprite.sprite.setTexture(contentsSprite.texture);
  contentsSprite.sprite.setPosition(x + 16, y + 16);
  contentsSprite.sprite.setColor(sf::Color(255, 255, 255, contents_opacity));
}

void Window::updateCursorRect()
{
  cursorSprite.visible = visible && !cursor_rect->isEmpty();
  if (windowSkin == nullptr || cursor_rect->isEmpty()) {
    return;
  }

  sf::Image src = windowSkin->renderTexture.getTexture().copyToImage();

  sf::Image buff;
  buff.create(cursor_rect->width.get(), cursor_rect->height.get(), sf::Color::Transparent);

  float xa = cursor_rect->width.get() / 32.0;
  float ya = cursor_rect->height.get() / 32.0;

  int lastLine = cursor_rect->height.get() - 1;
  for (int i = 0; i < cursor_rect->width.get(); i++) {
    buff.setPixel(i, 0, src.getPixel(128 + i / xa, 64));
    buff.setPixel(i, lastLine, src.getPixel(128 + i / xa, 95));
  }

  int lastCol = cursor_rect->width.get() - 1;
  for (int i = 0; i < cursor_rect->height.get(); i++) {
    buff.setPixel(0, i, src.getPixel(128, 64 + i / ya));
    buff.setPixel(lastCol, i, src.getPixel(159, 64 + i / ya));
  }

  int limitX = cursor_rect->width.get() - 1;
  int limitY = cursor_rect->height.get() - 1;
  xa = 30.0 / (limitX - 1);
  ya = 30.0 / (limitY - 1);
  for (int i = 1; i < limitX; i++) {
    for (int j = 1; j < limitY; j++) {
      buff.setPixel(
        i, j, src.getPixel(129 + std::floor((i - 1) * xa), 65 + std::floor((j - 1) * ya))
      );
    }
  }

  cursorSprite.texture.loadFromImage(buff);
  cursorSprite.sprite.setTexture(cursorSprite.texture);

  cursorSprite.sprite.setPosition(x + 16 + cursor_rect->x.get(), y + 16 + cursor_rect->y.get());
  cursor_rect->markAsClean();
}

void Window::updateWindowSpriteZ()
{
  frame.setZ(z);
  contentsSprite.setZ(z + 2);
  cursorSprite.setZ(z + 1);
  Lists::Instance().markZOrderDirty();
}
void Window::updateWindowSprite()
{
  frame.visible = visible;
  contentsSprite.visible = visible;
  cursorSprite.visible = visible;
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

}  // namespace Eng