#pragma once

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cmath>

#include "engnine/Bitmap.hpp"
#include "engnine/Drawable.hpp"
#include "engnine/Engine.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Viewport.hpp"

namespace Eng {

class Window : Drawable {
 public:

  Window(Viewport *viewport = 0) :
      windowSkin(nullptr),
      contents(nullptr),
      cursor_rect(new Rect(0, 0, 0, 0))
  {
    Eng::Engine::getInstance().addDrawable(this);
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
  }

  ~Window()
  {
    // free(windowSkin);
    // free(contents);
    // free(cursor_rect);
  }

  int getZPosition() const override
  {
    return z;
  }

  void update() override
  {
    if (width < 1 || height < 1) {
      return;
    }
    if (skinDirty) {
      Log::out() << "updateSkin";
      updateBackgroundSprite();
      updateBorder();
      updateCursorRect();
      skinDirty = false;
      cursor_rect->markAsClean();
    }
    if (cursor_rect->isDirty()) {
      // Log::out() << "updateCursorRect";
      updateCursorRect();
      cursor_rect->markAsClean();
    }
    if (contentsDirty) {
      updateContentsSprite();
      contentsDirty = false;
    }
  }

  void draw(sf::RenderTexture &rd) override
  {
    rd.draw(backgroundSprite, sf::BlendAlpha);
    rd.draw(borderSprite, sf::BlendAlpha);
    if (!cursor_rect->isEmpty()) {
      rd.draw(cursorSprite, sf::BlendAlpha);
    }
    rd.draw(contentsSprite, sf::BlendAlpha);
  }

  Bitmap *getWindowSkin()
  {
    return windowSkin;
  }
  void setWindowSkin(Bitmap *value)
  {
    if (windowSkin == value) {
      return;
    }

    windowSkin = value;
  }

  Bitmap *getContents()
  {
    return contents;
  }
  void setContents(Bitmap *v)
  {
    if (contents == v) {
      return;
    }

    contents = v;
  }

  bool getter_stretch() { return stretch; }
  void setter_stretch(bool v) { stretch = v; }

  Rect *getter_cursor_rect() { return cursor_rect; }
  void setter_cursor_rect(Rect *v) { cursor_rect = v; }

  bool getter_active() { return active; }
  void setter_active(bool v) { active = v; }

  bool getter_visible() { return visible; }
  void setter_visible(bool v) { visible = v; }

  bool getter_pause() { return pause; }
  void setter_pause(bool v) { pause = v; }

  int getX() { return x; }
  void setX(int v)
  {
    x = v;
    skinDirty = true;
  }

  int getY() { return y; }
  void setY(int v)
  {
    y = v;
    skinDirty = true;
  }

  int getWidth() { return width; }
  void setWidth(int v)
  {
    width = v;
    skinDirty = true;
  }

  int getHeight() { return height; }
  void setHeight(int v)
  {
    height = v;
    skinDirty = true;
  }

  int getZ() { return z; }
  void setZ(int v)
  {
    z = v;
    Eng::Engine::getInstance().markZOrderDirty();
  }

  int getter_ox() { return ox; }
  void setter_ox(int v)
  {
    ox = v;
    contentsDirty = true;
    Log::out() << "(setter_ox)";
  }

  int getter_oy() { return oy; }
  void setter_oy(int v)
  {
    oy = v;
    contentsDirty = true;
    Log::out() << "(setter_oy)";
  }

  int getter_opacity() { return opacity; }
  void setter_opacity(int v) { opacity = v; }

  int getter_back_opacity() { return back_opacity; }
  void setter_back_opacity(int v) { back_opacity = v; }

  int getter_contents_opacity() { return contents_opacity; }
  void setter_contents_opacity(int v) { contents_opacity = v; }

  void method_dispose()
  {
    isDisposed = true;
  }

  bool method_disposed()
  {
    return isDisposed;
  }

 private:
  Bitmap *windowSkin;
  Bitmap *contents;
  bool stretch;
  Rect *cursor_rect;
  bool active;
  bool visible;
  bool pause;
  int x;
  int y;
  int width;
  int height;
  int z;
  int ox;
  int oy;
  int opacity;
  int back_opacity;
  int contents_opacity;
  bool isDisposed;

  sf::Sprite backgroundSprite;
  sf::Sprite contentsSprite;

  sf::Sprite cursorSprite;
  sf::Texture cursorTexture;

  sf::Sprite borderSprite;
  sf::Texture borderTexture;

  // bool dirty;
  bool skinDirty;
  bool contentsDirty;

  void updateBackgroundSprite()
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

    Log::out() << "update()";
    Log::out() << "Rect width: " << cursor_rect->getter_width();
    Log::out() << "Rect height: " << cursor_rect->getter_height();
  }

  void updateContentsSprite()
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
  }

  void updateCursorRect()
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

  void updateBorder()
  {
    if (windowSkin == nullptr) {
      return;
    }
    Log::out() << " - [[updateBorder]]";
    constexpr int BORDER_START_X = 128;
    constexpr int BORDER_START_Y = 0;
    constexpr int BORDER_END_X = 192;
    constexpr int BORDER_END_Y = 64;
    constexpr int CORNER_SIZE = 16;
    constexpr int BORDER_THICK = 16;
    constexpr int BORDER_SIZE = 64;
    constexpr int BORDER_LINE = BORDER_SIZE - CORNER_SIZE * 2;

    sf::Image src = windowSkin->renderTexture.getTexture().copyToImage();
    sf::Texture srcTexture;
    srcTexture.loadFromImage(src);

    sf::RenderTexture rd;
    rd.create(width, height);
    rd.clear(sf::Color::Transparent);

    sf::IntRect dstRect(BORDER_START_X, BORDER_START_Y, CORNER_SIZE, CORNER_SIZE);

    sf::Sprite leftTop;
    leftTop.setTexture(srcTexture);
    leftTop.setTextureRect(dstRect);
    leftTop.setPosition(0, 0);
    rd.draw(leftTop);
    rd.display();

    sf::Sprite rightTop;
    rightTop.setTexture(srcTexture);
    dstRect.left = BORDER_END_X - CORNER_SIZE;
    rightTop.setTextureRect(dstRect);
    rightTop.setPosition(width - CORNER_SIZE, 0);
    rd.draw(rightTop);
    rd.display();

    sf::Sprite rightBottom;
    rightBottom.setTexture(srcTexture);
    dstRect.left = BORDER_END_X - CORNER_SIZE;
    dstRect.top = BORDER_END_Y - CORNER_SIZE;
    rightBottom.setTextureRect(dstRect);
    rightBottom.setPosition(width - CORNER_SIZE, height - CORNER_SIZE);
    rd.draw(rightBottom);
    rd.display();

    sf::Sprite leftBottom;
    leftBottom.setTexture(srcTexture);
    dstRect.left = BORDER_START_X;
    dstRect.top = BORDER_END_Y - CORNER_SIZE;
    leftBottom.setTextureRect(dstRect);
    leftBottom.setPosition(0, height - CORNER_SIZE);
    rd.draw(leftBottom);
    rd.display();

    sf::Sprite top;
    top.setTexture(srcTexture);
    dstRect = sf::IntRect(
      BORDER_START_X + CORNER_SIZE,
      BORDER_START_Y,
      BORDER_LINE,
      BORDER_THICK
    );
    top.setTextureRect(dstRect);
    int limitX = std::floor((width - CORNER_SIZE * 2) / BORDER_LINE);
    for (int i = 0; i < limitX; i++) {
      top.setPosition(CORNER_SIZE + BORDER_LINE * i, 0);
      rd.draw(top);
      rd.display();
    }
    int stopX = CORNER_SIZE + BORDER_LINE * limitX;
    int missingX = width - CORNER_SIZE - stopX;
    if (missingX > 0) {
      dstRect.width = missingX;
      top.setTextureRect(dstRect);
      top.setPosition(stopX, 0);
      rd.draw(top);
      rd.display();
    }

    sf::Sprite bottom;
    bottom.setTexture(srcTexture);
    dstRect = sf::IntRect(
      BORDER_START_X + CORNER_SIZE,
      BORDER_END_Y - CORNER_SIZE,
      BORDER_LINE,
      BORDER_THICK
    );
    bottom.setTextureRect(dstRect);
    limitX = std::floor((width - CORNER_SIZE * 2) / BORDER_LINE);
    int _y = height - CORNER_SIZE;
    for (int i = 0; i < limitX; i++) {
      bottom.setPosition(CORNER_SIZE + BORDER_LINE * i, _y);
      rd.draw(bottom);
      rd.display();
    }
    stopX = CORNER_SIZE + BORDER_LINE * limitX;
    missingX = width - CORNER_SIZE - stopX;
    if (missingX > 0) {
      dstRect.width = missingX;
      bottom.setTextureRect(dstRect);
      bottom.setPosition(stopX, _y);
      rd.draw(bottom);
      rd.display();
    }

    sf::Sprite left;
    dstRect = sf::IntRect(BORDER_START_X, BORDER_START_Y + CORNER_SIZE, BORDER_THICK, BORDER_LINE);
    left.setTexture(srcTexture);
    left.setTextureRect(dstRect);
    int limitY = std::floor((height - CORNER_SIZE * 2) / BORDER_LINE);
    for (int i = 0; i < limitY; i++) {
      left.setPosition(0, CORNER_SIZE + BORDER_LINE * i);
      rd.draw(left);
      rd.display();
    }
    int stopY = CORNER_SIZE + BORDER_LINE * limitY;
    int missingY = height - CORNER_SIZE - stopY;
    if (missingY > 0) {
      dstRect.height = missingY;
      left.setTextureRect(dstRect);
      left.setPosition(0, stopY);
      rd.draw(left);
      rd.display();
    }

    sf::Sprite right;
    dstRect = sf::IntRect(BORDER_END_X - BORDER_THICK, BORDER_START_Y + CORNER_SIZE, BORDER_THICK, BORDER_LINE);
    right.setTexture(srcTexture);
    right.setTextureRect(dstRect);
    int _x = width - BORDER_THICK;
    for (int i = 0; i < limitY; i++) {
      right.setPosition(_x, CORNER_SIZE + BORDER_LINE * i);
      rd.draw(right);
      rd.display();
    }
    stopY = CORNER_SIZE + BORDER_LINE * limitY;
    missingY = height - CORNER_SIZE - stopY;
    if (missingY > 0) {
      dstRect.height = missingY;
      right.setTextureRect(dstRect);
      right.setPosition(_x, stopY);
      rd.draw(right);
      rd.display();
    }

    borderTexture = rd.getTexture();
    borderSprite.setTexture(borderTexture);
    borderSprite.setPosition(x, y);
  }
};

}  // namespace Eng