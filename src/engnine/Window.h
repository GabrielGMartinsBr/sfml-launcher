#pragma once

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "engnine/Bitmap.h"
#include "engnine/EngineBase.hpp"
#include "engnine/IViewportChild.h"
#include "engnine/OnUpdate.h"
#include "engnine/Rect.hpp"
#include "engnine/Viewport.hpp"

namespace Eng {

class Window : public OnUpdate, IViewportChild, public EngineBase {
 public:

  Window(Viewport *viewport = nullptr);

  Window(VALUE rbObj, Viewport *viewport = nullptr);

  ~Window();

  // Engine

  inline Viewport *getViewport() const override;

  int getZIndex() const override;

  bool shouldRender() const override;

  void onUpdate() override;

  void onRender(sf::RenderTexture &rd) override;

  // Rgss

  Bitmap *getter_windowskin();
  void setter_windowskin(Bitmap *value);

  Bitmap *getter_contents();
  void setter_contents(Bitmap *v);

  // Prop stretch

  VALUE getter_stretch();
  VALUE setter_stretch(VALUE v);

  bool stretch();
  void stretch(bool v);

  // Prop cursor_rect

  Rect *getter_cursor_rect();
  void setter_cursor_rect(Rect *v);

  // Prop active

  bool getActive();
  void setActive(bool v);

  VALUE getter_active();
  VALUE setter_active(VALUE v);

  // Property visible

  bool getVisible();
  void setVisible(bool v);

  VALUE getter_visible();
  VALUE setter_visible(VALUE v);

  // Property pause

  bool getPause();
  void setPause(bool v);

  VALUE getter_pause();
  VALUE setter_pause(VALUE v);

  int getX();
  void setX(int v);
  VALUE setter_x(VALUE v);

  int getY();
  void setY(int v);
  VALUE setter_y(VALUE v);

  int getWidth();
  void setWidth(int v);
  VALUE setter_width(VALUE v);

  int getHeight();
  void setHeight(int v);
  VALUE setter_height(VALUE v);

  int getZ();
  void setZ(int v);
  VALUE setter_z(VALUE v);

  int getter_ox();
  void setter_ox(int v);
  VALUE setter_ox(VALUE v);

  int getter_oy();
  void setter_oy(int v);
  VALUE setter_oy(VALUE v);

  int getter_opacity();
  void setter_opacity(int v);
  VALUE setter_opacity(VALUE v);

  int getter_back_opacity();
  void setter_back_opacity(int v);
  VALUE setter_back_opacity(VALUE v);

  int getter_contents_opacity();
  void setter_contents_opacity(int v);
  VALUE setter_contents_opacity(VALUE v);

  /*  Methods */

  Viewport *method_viewport();

  void method_dispose();

  bool method_disposed();

  void method_update();

 private:
  Viewport *viewport;
  Bitmap *windowSkin;
  Bitmap *contents;
  bool _stretch;
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
  bool addedToEngineCycles;

  int cursorAniAlphaId;

  void bindRubyVars();

  void addToEngineCycles();
  void removeFromEngineCycles();

  void updateBackgroundSprite();

  void updateContentsSprite();

  void updateCursorRect();

  void updateBorder();
};

}  // namespace Eng