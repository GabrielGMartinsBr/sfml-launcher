#pragma once

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "engnine/Bitmap.h"
#include "engnine/Drawable.hpp"
#include "engnine/EngineBase.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Viewport.hpp"

namespace Eng {

class Window : public EngineBase, Drawable {
 public:

  Window(Viewport *viewport = 0);

  ~Window();

  int getZPosition() const override;

  bool shouldRender() const override;

  void update() override;

  void draw(sf::RenderTexture &rd) override;

  Bitmap *getWindowSkin();
  void setWindowSkin(Bitmap *value);

  Bitmap *getContents();
  void setContents(Bitmap *v);

  bool getter_stretch();
  void setter_stretch(bool v);

  Rect *getter_cursor_rect();
  void setter_cursor_rect(Rect *v);

  bool getter_active();
  void setter_active(bool v);

  bool getter_visible();
  void setter_visible(bool v);

  bool getter_pause();
  void setter_pause(bool v);

  int getX();
  void setX(int v);

  int getY();
  void setY(int v);

  int getWidth();
  void setWidth(int v);

  int getHeight();
  void setHeight(int v);

  int getZ();
  void setZ(int v);

  int getter_ox();
  void setter_ox(int v);

  int getter_oy();
  void setter_oy(int v);

  int getter_opacity();
  void setter_opacity(int v);

  int getter_back_opacity();
  void setter_back_opacity(int v);

  int getter_contents_opacity();
  void setter_contents_opacity(int v);

  void method_dispose();

  bool method_disposed();

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

  bool skinDirty;
  bool contentsDirty;

  void updateBackgroundSprite();

  void updateContentsSprite();

  void updateCursorRect();

  void updateBorder();
};

}  // namespace Eng