
#include "Window.h"

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cmath>

#include "NumberUtils.hpp"
#include "engnine/Bitmap.h"
#include "engnine/Engine.h"
#include "engnine/Rect.hpp"
#include "engnine/Viewport.hpp"
#include "integrator/It_Bitmap.hpp"
#include "integrator/It_Rect.hpp"
#include "integrator/It_Viewport.hpp"

namespace Eng {

static const uint8_t cursorAniAlpha[] = {
  /* Fade out */
  0xFF, 0xF7, 0xEF, 0xE7, 0xDF, 0xD7, 0xCF, 0xC7,
  0xBF, 0xB7, 0xAF, 0xA7, 0x9F, 0x97, 0x8F, 0x87,
  /* Fade in */
  0x7F, 0x87, 0x8F, 0x97, 0x9F, 0xA7, 0xAF, 0xB7,
  0xBF, 0xC7, 0xCF, 0xD7, 0xDF, 0xE7, 0xEF, 0xF7
};
static const int cursorAniAlphaN = sizeof(cursorAniAlpha);

Window::Window(Viewport *viewport) :
    Window(Qnil, viewport)
{
}

Window::Window(VALUE rbObj, Viewport *viewport) :
    EngineBase(rbObj),
    viewport(viewport),
    windowSkin(nullptr),
    contents(nullptr),
    cursor_rect(new Rect(0, 0, 0, 0))
{
  contentsDirty = true;
  skinDirty = true;

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

  if (rbObj != Qnil) {
    bindRubyProps();
  }

  addToEngineCycles();
}

Window::~Window()
{
  removeFromEngineCycles();
}

// Engine

int Window::getZIndex() const
{
  return z;
}

bool Window::shouldRender() const
{
  return !isDisposed && visible;
}

void Window::onUpdate()
{
  if (width < 1 || height < 1) {
    return;
  }
  if (skinDirty) {
    updateBackgroundSprite();
    updateBorder();
    updateCursorRect();
    skinDirty = false;
    cursor_rect->markAsClean();
  }
  if (cursor_rect->isDirty()) {
    updateCursorRect();
    cursor_rect->markAsClean();
  }
  if (contentsDirty) {
    updateContentsSprite();
    contentsDirty = false;
  }
}

void Window::onRender(sf::RenderTexture &rd)
{
  rd.draw(backgroundSprite, sf::BlendAlpha);
  rd.draw(borderSprite, sf::BlendAlpha);
  if (!cursor_rect->isEmpty()) {
    rd.draw(cursorSprite, sf::BlendAlpha);
  }
  rd.draw(contentsSprite, sf::BlendAlpha);
}

/*
  Rgss
*/

// Bind props ruby object to instance object

void Window::bindRubyProps()
{
  if (rbObj == Qnil) {
    std::runtime_error("Window doesn't have rbObj defined.");
  }

  if (viewport != nullptr && viewport->rbObj == Qnil) {
    viewport->rbObj = It::Viewport::createRubyObject(viewport);
    rb_iv_set(rbObj, "@viewport", viewport->rbObj);
  }

  if (cursor_rect->rbObj == Qnil) {
    cursor_rect->rbObj = It::Rect::createRubyObject(cursor_rect);
  }

  rb_iv_set(rbObj, "@cursor_rect", cursor_rect->rbObj);
}

/*
  Properties
*/

Bitmap *Window::getter_windowskin()
{
  return windowSkin;
}
void Window::setter_windowskin(Bitmap *value)
{
  if (windowSkin == value) {
    return;
  }

  if (value->rbObj == Qnil) {
    value->rbObj = It::Bitmap::createRubyObject(value);
  }

  windowSkin = value;
  rb_iv_set(rbObj, "@windowskin", windowSkin->rbObj);
}

Bitmap *Window::getter_contents()
{
  return contents;
}
void Window::setter_contents(Bitmap *value)
{
  if (contents == value) {
    return;
  }

  if (value->rbObj == Qnil) {
    value->rbObj = It::Bitmap::createRubyObject(value);
  }

  contents = value;
  rb_iv_set(rbObj, "@contents", contents->rbObj);
}

bool Window::getter_stretch() { return stretch; }
void Window::setter_stretch(bool v) { stretch = v; }

Rect *Window::getter_cursor_rect() { return cursor_rect; }
void Window::setter_cursor_rect(Rect *v) { cursor_rect = v; }

bool Window::getter_active() { return active; }
void Window::setter_active(bool v) { active = v; }

bool Window::getter_visible() { return visible; }
void Window::setter_visible(bool v) { visible = v; }

bool Window::getter_pause() { return pause; }
void Window::setter_pause(bool v) { pause = v; }

int Window::getX() { return x; }
void Window::setX(int v)
{
  x = v;
  skinDirty = true;
}

int Window::getY() { return y; }
void Window::setY(int v)
{
  y = v;
  skinDirty = true;
}

int Window::getWidth() { return width; }
void Window::setWidth(int v)
{
  width = v;
  skinDirty = true;
}

int Window::getHeight() { return height; }
void Window::setHeight(int v)
{
  height = v;
  skinDirty = true;
}

int Window::getZ() { return z; }
void Window::setZ(int v)
{
  z = v;
  Engine::getInstance().markZOrderDirty();
}

int Window::getter_ox() { return ox; }
void Window::setter_ox(int v)
{
  ox = v;
  contentsDirty = true;
}

int Window::getter_oy() { return oy; }
void Window::setter_oy(int v)
{
  oy = v;
  contentsDirty = true;
}

int Window::getter_opacity() { return opacity; }
void Window::setter_opacity(int v)
{
  int value = Num::clamp(v, 0, 255);
  if (opacity != value) {
    opacity = value;
    skinDirty = true;
  }
}

int Window::getter_back_opacity() { return back_opacity; }
void Window::setter_back_opacity(int v)
{
  int value = Num::clamp(v, 0, 255);
  if (back_opacity != value) {
    back_opacity = value;
  }
}

int Window::getter_contents_opacity() { return contents_opacity; }
void Window::setter_contents_opacity(int v)
{
  int value = Num::clamp(v, 0, 255);
  if (contents_opacity != value) {
    contents_opacity = value;
    contentsDirty = true;
  }
}

/*
  Methods
*/

// Method viewport

Viewport *Window::method_viewport()
{
  return viewport;
}

// Method dispose

void Window::method_dispose()
{
  isDisposed = true;
  removeFromEngineCycles();
}

// Method disposed?

bool Window::method_disposed()
{
  return isDisposed;
}

void Window::method_update()
{
  cursorAniAlphaId = (cursorAniAlphaId + 1) % cursorAniAlphaN;
  cursorSprite.setColor(sf::Color(255, 255, 255, cursorAniAlpha[cursorAniAlphaId]));
  contentsDirty = contents->dirty;
}

/*
  Private
*/

void Window::addToEngineCycles()
{
  if (addedToEngineCycles) {
    return;
  }
  Engine::getInstance().addToUpdateList(this);
  Engine::getInstance().addToRenderList(this);
  addedToEngineCycles = true;
}

void Window::removeFromEngineCycles()
{
  if (!addedToEngineCycles) {
    return;
  }
  Engine::getInstance().removeFromUpdateList(this);
  Engine::getInstance().removeFromRenderList(this);
  addedToEngineCycles = false;
}

void Window::updateBackgroundSprite()
{
  if (windowSkin == nullptr) {
    return;
  }

  float scaleX = (width - 2) / 128.0;
  float scaleY = (height - 2) / 128.0;

  backgroundSprite.setTexture(
    windowSkin->renderTexture.getTexture()
  );

  sf::IntRect dstRect(0, 0, 128, 128);
  backgroundSprite.setTextureRect(dstRect);

  backgroundSprite.setScale(scaleX, scaleY);
  backgroundSprite.setPosition(x + 1, y + 1);

  backgroundSprite.setColor(sf::Color(255, 255, 255, back_opacity));
}

void Window::updateContentsSprite()
{
  if (contents == nullptr) {
    return;
  }

  const sf::Texture &texture = contents->renderTexture.getTexture();

  sf::RenderTexture *rd = new sf::RenderTexture();
  rd->create(width, height);
  rd->clear(sf::Color::Transparent);
  // rd.draw(texture);

  sf::Sprite *spr = new sf::Sprite();
  spr->setTexture(texture);
  // spr->setPosition(x, y);

  rd->draw(*spr);
  rd->display();
  const sf::Texture *tex = &rd->getTexture();

  contentsSprite.setTexture(*tex);
  contentsSprite.setPosition(x + 16, y + 16);
  contentsSprite.setColor(sf::Color(255, 255, 255, contents_opacity));
}

void Window::updateCursorRect()
{
  if (windowSkin == nullptr || cursor_rect->isEmpty()) {
    return;
  }

  sf::Image src = windowSkin->renderTexture.getTexture().copyToImage();

  sf::Image buff;
  buff.create(cursor_rect->getter_width(), cursor_rect->getter_height(), sf::Color::Transparent);

  float xa = cursor_rect->getter_width() / 32.0;
  float ya = cursor_rect->getter_height() / 32.0;

  int lastLine = cursor_rect->getter_height() - 1;
  for (int i = 0; i < cursor_rect->getter_width(); i++) {
    buff.setPixel(i, 0, src.getPixel(128 + i / xa, 64));
    buff.setPixel(i, lastLine, src.getPixel(128 + i / xa, 95));
  }

  int lastCol = cursor_rect->getter_width() - 1;
  for (int i = 0; i < cursor_rect->getter_height(); i++) {
    buff.setPixel(0, i, src.getPixel(128, 64 + i / ya));
    buff.setPixel(lastCol, i, src.getPixel(159, 64 + i / ya));
  }

  int limitX = cursor_rect->getter_width() - 1;
  int limitY = cursor_rect->getter_height() - 1;
  xa = 30.0 / (limitX - 1);
  ya = 30.0 / (limitY - 1);
  for (int i = 1; i < limitX; i++) {
    for (int j = 1; j < limitY; j++) {
      buff.setPixel(
        i, j, src.getPixel(129 + std::floor((i - 1) * xa), 65 + std::floor((j - 1) * ya))
      );
    }
  }

  cursorTexture.loadFromImage(buff);

  cursorSprite.setPosition(x + 16 + cursor_rect->getter_x(), y + 16 + cursor_rect->getter_y());
  cursorSprite.setTexture(cursorTexture);
  cursor_rect->markAsClean();
}

void Window::updateBorder()
{
  if (windowSkin == nullptr) {
    return;
  }
  constexpr int BORDER_START_X = 128;
  constexpr int BORDER_START_Y = 0;
  constexpr int BORDER_END_X = 192;
  constexpr int BORDER_END_Y = 64;
  constexpr int CORNER_SIZE = 16;
  constexpr int BORDER_THICK = 16;
  constexpr int BORDER_SIZE = 64;
  constexpr int BORDER_LINE = BORDER_SIZE - CORNER_SIZE * 2;

  sf::Texture srcTexture(windowSkin->getTexture());

  sf::RenderTexture rd;
  rd.create(width, height);
  rd.clear(sf::Color::Transparent);

  sf::IntRect topLeft(
    BORDER_START_X,
    BORDER_START_Y,
    CORNER_SIZE,
    CORNER_SIZE
  );
  sf::IntRect topRight(
    BORDER_END_X - CORNER_SIZE,
    BORDER_START_Y,
    CORNER_SIZE,
    CORNER_SIZE
  );
  sf::IntRect bottomRight(
    BORDER_END_X - CORNER_SIZE,
    BORDER_END_Y - CORNER_SIZE,
    CORNER_SIZE,
    CORNER_SIZE
  );
  sf::IntRect bottomLeft(
    BORDER_START_X,
    BORDER_END_Y - CORNER_SIZE,
    CORNER_SIZE,
    CORNER_SIZE
  );
  sf::IntRect top(
    BORDER_START_X + CORNER_SIZE,
    BORDER_START_Y,
    BORDER_LINE,
    BORDER_THICK
  );
  sf::IntRect bottom(
    BORDER_START_X + CORNER_SIZE,
    BORDER_END_Y - CORNER_SIZE,
    BORDER_LINE,
    BORDER_THICK
  );
  sf::IntRect left(
    BORDER_START_X,
    BORDER_START_Y + CORNER_SIZE,
    BORDER_THICK,
    BORDER_LINE
  );
  sf::IntRect right(
    BORDER_END_X - BORDER_THICK,
    BORDER_START_Y + CORNER_SIZE,
    BORDER_THICK,
    BORDER_LINE
  );

  sf::Sprite borderSpr;
  borderSpr.setTexture(srcTexture);

  // Top Left

  borderSpr.setTextureRect(topLeft);
  borderSpr.setPosition(0, 0);
  rd.draw(borderSpr);
  rd.display();

  // Top Right

  borderSpr.setTextureRect(topRight);
  borderSpr.setPosition(width - CORNER_SIZE, 0);
  rd.draw(borderSpr);
  rd.display();

  // Bottom Right

  borderSpr.setTextureRect(bottomRight);
  borderSpr.setPosition(width - CORNER_SIZE, height - CORNER_SIZE);
  rd.draw(borderSpr);
  rd.display();

  // Bottom Left

  borderSpr.setTextureRect(bottomLeft);
  borderSpr.setPosition(0, height - CORNER_SIZE);
  rd.draw(borderSpr);
  rd.display();

  // Top

  borderSpr.setTextureRect(top);
  int limitX = std::floor((width - CORNER_SIZE * 2) / BORDER_LINE);
  for (int i = 0; i < limitX; i++) {
    borderSpr.setPosition(CORNER_SIZE + BORDER_LINE * i, 0);
    rd.draw(borderSpr);
    rd.display();
  }

  int stopX = CORNER_SIZE + BORDER_LINE * limitX;
  int missingX = width - CORNER_SIZE - stopX;
  if (missingX > 0) {
    top.width = missingX;
    borderSpr.setTextureRect(top);
    borderSpr.setPosition(stopX, 0);
    rd.draw(borderSpr);
    rd.display();
  }

  // Bottom

  borderSpr.setTextureRect(bottom);
  int _y = height - CORNER_SIZE;
  for (int i = 0; i < limitX; i++) {
    borderSpr.setPosition(CORNER_SIZE + BORDER_LINE * i, _y);
    rd.draw(borderSpr);
    rd.display();
  }

  if (missingX > 0) {
    bottom.width = missingX;
    borderSpr.setTextureRect(bottom);
    borderSpr.setPosition(stopX, _y);
    rd.draw(borderSpr);
    rd.display();
  }

  // Left

  borderSpr.setTextureRect(left);
  int limitY = std::floor((height - CORNER_SIZE * 2) / BORDER_LINE);
  for (int i = 0; i < limitY; i++) {
    borderSpr.setPosition(0, CORNER_SIZE + BORDER_LINE * i);
    rd.draw(borderSpr);
    rd.display();
  }

  int stopY = CORNER_SIZE + BORDER_LINE * limitY;
  int missingY = height - CORNER_SIZE - stopY;
  if (missingY > 0) {
    left.height = missingY;
    borderSpr.setTextureRect(left);
    borderSpr.setPosition(0, stopY);
    rd.draw(borderSpr);
    rd.display();
  }

  // Right

  borderSpr.setTextureRect(right);
  int _x = width - BORDER_THICK;
  for (int i = 0; i < limitY; i++) {
    borderSpr.setPosition(_x, CORNER_SIZE + BORDER_LINE * i);
    rd.draw(borderSpr);
    rd.display();
  }

  if (missingY > 0) {
    right.height = missingY;
    borderSpr.setTextureRect(right);
    borderSpr.setPosition(_x, stopY);
    rd.draw(borderSpr);
    rd.display();
  }

  borderTexture = rd.getTexture();
  borderSprite.setTexture(borderTexture);
  borderSprite.setPosition(x, y);

  borderSprite.setColor(sf::Color(255, 255, 255, opacity));
}

}  // namespace Eng