
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
#include "engnine/Lists.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Viewport.hpp"
#include "integrator/Convert.hpp"
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

  if (hasRbObj()) {
    bindRubyVars();
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
  if (contentsDirty || (contents && contents->dirty)) {
    updateContentsSprite();
    contentsDirty = false;
    if (contents) {
      contents->dirty = false;
    }
  }
}

void Window::onRender(sf::RenderTexture &rd)
{
  rd.draw(backgroundSprite);
  rd.draw(borderSprite);
  if (!cursor_rect->isEmpty()) {
    rd.draw(cursorSprite);
  }
  rd.draw(contentsSprite);
}

/*
  Rgss
*/

// Bind props ruby object to instance object

void Window::bindRubyVars()
{
  if (rbObj == Qnil) {
    std::runtime_error("Window doesn't have rbObj defined.");
  }

  VALUE viewportVal = Qnil;
  if (viewport) {
    if (!viewport->hasRbObj()) {
      viewport->rbObj = It::Viewport::getRubyObject(viewport);
      viewport->bindRubyVars();
    }
    viewportVal = viewport->rbObj;
  }

  if (cursor_rect->rbObj == Qnil) {
    cursor_rect->rbObj = It::Rect::createRubyObject(cursor_rect);
    cursor_rect->bindRubyVars();
  }

  rb_iv_set(rbObj, "@viewport", viewportVal);
  rb_iv_set(rbObj, "@cursor_rect", cursor_rect->rbObj);

  rb_iv_set(rbObj, "@windowSkin", Qnil);
  rb_iv_set(rbObj, "@contents", Qnil);

  rb_iv_set(rbObj, "@stretch", Convert::toRubyBool(_stretch));
  rb_iv_set(rbObj, "@visible", Convert::toRubyBool(visible));
  rb_iv_set(rbObj, "@active", Convert::toRubyBool(active));
  rb_iv_set(rbObj, "@pause", Convert::toRubyBool(pause));

  rb_iv_set(rbObj, "@x", Convert::toRubyNumber(x));
  rb_iv_set(rbObj, "@y", Convert::toRubyNumber(y));
  rb_iv_set(rbObj, "@width", Convert::toRubyNumber(width));
  rb_iv_set(rbObj, "@height", Convert::toRubyNumber(height));
  rb_iv_set(rbObj, "@z", Convert::toRubyNumber(z));
  rb_iv_set(rbObj, "@ox", Convert::toRubyNumber(ox));
  rb_iv_set(rbObj, "@oy", Convert::toRubyNumber(oy));
  rb_iv_set(rbObj, "@opacity", Convert::toRubyNumber(opacity));
  rb_iv_set(rbObj, "@back_opacity", Convert::toRubyNumber(back_opacity));
  rb_iv_set(rbObj, "@contents_opacity", Convert::toRubyNumber(contents_opacity));
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

  if (value == nullptr) {
    windowSkin = nullptr;
    setInstanceVar("@windowskin", Qnil);
    return;
  }

  value->initRubyObj();
  windowSkin = value;
  setInstanceVar("@windowskin", windowSkin->rbObj);
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

  if (value == nullptr) {
    contents = nullptr;
    setInstanceVar("@contents", Qnil);
    return;
  }

  value->initRubyObj();
  contents = value;
  setInstanceVar("@contents", contents->rbObj);
}

VALUE Window::getter_stretch()
{
  return Convert::toRubyBool(_stretch);
}

VALUE Window::setter_stretch(VALUE v)
{
  bool value = Convert::toCBool(v);
  if (_stretch == value) return v;
  _stretch = value;
  setInstanceVar("@stretch", v);
  return v;
}

bool Window::stretch() { return _stretch; }
void Window::stretch(bool v)
{
  if (_stretch == v) return;
  _stretch = v;
  setInstanceVar("@stretch", Convert::toRubyBool(v));
}

Rect *Window::getter_cursor_rect() { return cursor_rect; }
void Window::setter_cursor_rect(Rect *v) { cursor_rect = v; }

/*
  Property active
*/

bool Window::getActive() { return active; }
void Window::setActive(bool v)
{
  if (active == v) return;
  active = v;
  setInstanceVar("@active", Convert::toRubyBool(v));
  skinDirty = true;
  contentsDirty = true;
}

VALUE Window::getter_active()
{
  return Convert::toRubyBool(active);
}

VALUE Window::setter_active(VALUE v)
{
  bool value = Convert::toCBool(v);
  if (active == value) return v;
  active = value;
  setInstanceVar("@active", v);
  skinDirty = true;
  contentsDirty = true;
  return v;
}

// --------------

/*
  Property visible
*/

bool Window::getVisible() { return visible; }
void Window::setVisible(bool v)
{
  if (visible == v) return;
  visible = v;
  setInstanceVar("@visible", Convert::toRubyBool(v));
}

VALUE Window::getter_visible()
{
  return Convert::toRubyBool(visible);
}

VALUE Window::setter_visible(VALUE v)
{
  bool value = Convert::toCBool(v);
  if (visible == value) return v;
  visible = value;
  setInstanceVar("@visible", v);
  return v;
}

// --------------

/*
  Property visible
*/

bool Window::getPause() { return pause; }
void Window::setPause(bool v)
{
  if (pause == v) return;
  pause = v;
  setInstanceVar("@pause", Convert::toRubyBool(v));
}

VALUE Window::getter_pause()
{
  return Convert::toRubyBool(pause);
}

VALUE Window::setter_pause(VALUE v)
{
  bool value = Convert::toCBool(v);
  if (pause == value) return v;
  pause = value;
  setInstanceVar("@pause", v);
  return v;
}

// --------------

int Window::getX() { return x; }
void Window::setX(int v)
{
  if (x == v) return;
  x = v;
  setInstanceVar("@x", Convert::toRubyNumber(v));
  skinDirty = true;
}
VALUE Window::setter_x(VALUE v)
{
  int value = Convert::toCInt2(v);
  if (x == value) return v;
  x = value;
  setInstanceVar("@x", v);
  skinDirty = true;
  return v;
}

int Window::getY() { return y; }
void Window::setY(int v)
{
  if (y == v) return;
  y = v;
  setInstanceVar("@y", Convert::toRubyNumber(v));
  skinDirty = true;
}
VALUE Window::setter_y(VALUE v)
{
  int value = Convert::toCInt2(v);
  if (y == value) return v;
  y = value;
  setInstanceVar("@y", v);
  skinDirty = true;
  return v;
}

/*  Property width */

int Window::getWidth() { return width; }
void Window::setWidth(int v)
{
  if (width == v) return;
  width = v;
  setInstanceVar("@width", Convert::toRubyNumber(v));
  skinDirty = true;
}
VALUE Window::setter_width(VALUE v)
{
  int value = Convert::toCInt2(v);
  if (width == value) return v;
  width = value;
  setInstanceVar("@width", v);
  skinDirty = true;
  return v;
}

/*  Property height */

int Window::getHeight() { return height; }
void Window::setHeight(int v)
{
  if (height == v) return;
  height = v;
  setInstanceVar("@height", Convert::toRubyNumber(v));
  skinDirty = true;
}
VALUE Window::setter_height(VALUE v)
{
  int value = Convert::toCInt2(v);
  if (height == value) return v;
  height = value;
  setInstanceVar("@height", v);
  skinDirty = true;
  return v;
}

/*  Property z */

int Window::getZ() { return z; }
void Window::setZ(int v)
{
  if (z == v) return;
  z = v;
  setInstanceVar("@z", Convert::toRubyNumber(v));
  Lists::Instance().markZOrderDirty();
}
VALUE Window::setter_z(VALUE v)
{
  int value = Convert::toCInt2(v);
  if (z == value) return v;
  z = value;
  setInstanceVar("@z", v);
  Lists::Instance().markZOrderDirty();
  return v;
}

/*  Property ox */

int Window::getter_ox() { return ox; }
void Window::setter_ox(int v)
{
  if (ox == v) return;
  ox = v;
  setInstanceVar("@ox", Convert::toRubyNumber(v));
  contentsDirty = true;
}
VALUE Window::setter_ox(VALUE v)
{
  int value = Convert::toCInt2(v);
  if (ox == value) return v;
  ox = value;
  setInstanceVar("@ox", v);
  contentsDirty = true;
  return v;
}

/*  Property oy */

int Window::getter_oy() { return oy; }
void Window::setter_oy(int v)
{
  if (oy == v) return;
  oy = v;
  setInstanceVar("@oy", Convert::toRubyNumber(v));
  contentsDirty = true;
}
VALUE Window::setter_oy(VALUE v)
{
  int value = Convert::toCInt2(v);
  if (oy == value) return v;
  oy = value;
  setInstanceVar("@oy", v);
  contentsDirty = true;
  return v;
}

/*  Property opacity */

int Window::getter_opacity() { return opacity; }
void Window::setter_opacity(int v)
{
  int value = Num::clamp(v, 0, 255);
  if (opacity == value) return;
  opacity = value;
  setInstanceVar("@opacity", Convert::toRubyNumber(value));
  skinDirty = true;
}
VALUE Window::setter_opacity(VALUE v)
{
  int value = Convert::toCInt2(v);
  value = Num::clamp(value, 0, 255);
  if (opacity == value) return v;
  v = Convert::toRubyNumber(value);
  opacity = value;
  setInstanceVar("@opacity", v);
  skinDirty = true;
  return v;
}

/*  Property back_opacity */

int Window::getter_back_opacity() { return back_opacity; }
void Window::setter_back_opacity(int v)
{
  int value = Num::clamp(v, 0, 255);
  if (back_opacity == value) return;
  back_opacity = value;
  setInstanceVar("@back_opacity", Convert::toRubyNumber(value));
}
VALUE Window::setter_back_opacity(VALUE v)
{
  int value = Convert::toCInt2(v);
  value = Num::clamp(value, 0, 255);
  if (back_opacity == value) return v;
  v = Convert::toRubyNumber(value);
  back_opacity = value;
  setInstanceVar("@back_opacity", v);
  return v;
}

/*  Property contents_opacity */

int Window::getter_contents_opacity() { return contents_opacity; }
void Window::setter_contents_opacity(int v)
{
  int value = Num::clamp(v, 0, 255);
  if (contents_opacity == value) return;
  contents_opacity = value;
  setInstanceVar("@contents_opacity", Convert::toRubyNumber(value));
  contentsDirty = true;
}
VALUE Window::setter_contents_opacity(VALUE v)
{
  int value = Convert::toCInt2(v);
  value = Num::clamp(value, 0, 255);
  if (contents_opacity == value) return v;
  v = Convert::toRubyNumber(value);
  contents_opacity = value;
  setInstanceVar("@contents_opacity", v);
  contentsDirty = true;
  return v;
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
  if (isDisposed || contents == nullptr || contents->disposed()) {
    return;
  }
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
  Lists::Instance().addUpdateEntry(this);
  Lists::Instance().addRenderEntry(this);
  addedToEngineCycles = true;
}

void Window::removeFromEngineCycles()
{
  if (!addedToEngineCycles) {
    return;
  }
  Lists::Instance().removeUpdateEntry(this);
  Lists::Instance().removeRenderEntry(this);
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

  cursorTexture.loadFromImage(buff);

  cursorSprite.setPosition(x + 16 + cursor_rect->x.get(), y + 16 + cursor_rect->y.get());
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