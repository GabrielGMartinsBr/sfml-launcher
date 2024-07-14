#pragma once

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Color.hpp"
#include "engnine/Bitmap.hpp"
#include "engnine/Engine.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Sprite.hpp"
#include "engnine/Viewport.hpp"

namespace Eng {

class Window {
 public:

  Window(Viewport *viewport = 0) :
      width(0),
      height(0),
      windowSkin(nullptr),
      contents(nullptr),
      sprContents(),
      backgroundSprite(),
      cursor_rect(new Rect(0, 0, 16, 16))
  {
    Eng::Engine::getInstance().addSprite(&backgroundSprite);
    Eng::Engine::getInstance().addSprite(&sprContents);
  }

  Bitmap *getWindowSkin() { return windowSkin; }
  void setWindowSkin(Bitmap *value)
  {
    if (windowSkin == value) {
      return;
    }

    windowSkin = value;

    if (value == nullptr) {
      return;
    }

    drawSkin();
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

    if (v == nullptr) {
      return;
    }

    drawContents();
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
    drawSkin();
    drawContents();
  }

  int getY() { return y; }
  void setY(int v)
  {
    y = v;
    drawSkin();
    drawContents();
  }

  int getWidth() { return width; }
  void setWidth(int v)
  {
    width = v;
    drawSkin();
    drawContents();
  }

  int getHeight() { return height; }
  void setHeight(int v)
  {
    height = v;
    drawSkin();
    drawContents();
  }

  int getZ() { return z; }
  void setZ(int v) { z = v; }

  int getter_ox() { return ox; }
  void setter_ox(int v) { ox = v; }

  int getter_oy() { return oy; }
  void setter_oy(int v) { oy = v; }

  int getter_opacity() { return opacity; }
  void setter_opacity(int v) { opacity = v; }

  int getter_back_opacity() { return back_opacity; }
  void setter_back_opacity(int v) { back_opacity = v; }

  int getter_contents_opacity() { return contents_opacity; }
  void setter_contents_opacity(int v) { contents_opacity = v; }

 private:
  Bitmap *windowSkin;
  Bitmap *contents;
  Sprite sprContents;
  Sprite backgroundSprite;
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

  void drawSkin()
  {
    if (width < 1 || height < 1 || windowSkin == nullptr) {
      return;
    }

    sf::Sprite spr;
    sf::Texture texture = windowSkin->renderTexture.getTexture();
    spr.setTexture(texture);

    sf::Vector2u textureSize = texture.getSize();
    float scaleX = width / 128.0;
    float scaleY = height / 128.0;
    spr.setScale(scaleX, scaleY);

    sf::IntRect dstRect(0, 0, 128, 128);
    spr.setTextureRect(dstRect);

    spr.setPosition(x, y);

    Bitmap *bg = new Bitmap(width, height);
    // bg->renderTexture.setSmooth(true);
    bg->renderTexture.draw(spr);
    bg->renderTexture.display();

    backgroundSprite.setBitmap(bg);

    // sf::RenderTexture &render = Eng::Engine::getInstance().getRenderer()->getRenderTexture();
    // render.draw(spr);
    // render.setSmooth(false);
    // render.display();
  }

  void drawContents()
  {
    if (width < 1 || height < 1 || contents == nullptr) {
      return;
    }

    sf::Sprite spr;
    sf::Texture texture = contents->renderTexture.getTexture();
    spr.setTexture(texture);

    // sf::Vector2u textureSize = texture.getSize();
    // float scaleX = (float)width / textureSize.x;
    // float scaleY = (float)height / textureSize.y;
    // spr.setScale(scaleX, scaleY);

    // sf::IntRect dstRect(0, 0, 128, 128);
    // spr.setTextureRect(dstRect);

    spr.setPosition(x, y);

    Bitmap *bp = new Bitmap(width, height);
    // bg->renderTexture.setSmooth(true);
    bp->renderTexture.draw(spr, sf::BlendNone);
    bp->renderTexture.display();

    sprContents.setBitmap(bp);
  }
};

}  // namespace Eng