#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include "engnine/Color.hpp"
#include "engnine/EngineBase.hpp"
#include "engnine/OnRender.h"
#include "engnine/Rect.hpp"
#include "engnine/Tone.hpp"

namespace Eng {
using app::Vector;

class Viewport : public OnRender, public EngineBase {
 public:

  /*
    ⇩⇩⇩ Public ⇩⇩⇩
  */

  Vector<OnRender*> children;
  sf::RenderTexture rd;
  sf::Texture texture;
  sf::Sprite sprite;
  sf::IntRect srcRect;

  Viewport(Rect* _rect) :
      Viewport(Qnil, _rect) { }

  Viewport(int x, int y, int width, int height) :
      Viewport(Qnil, x, y, width, height) { }

  Viewport(VALUE rbObj, int x, int y, int width, int height);

  Viewport(VALUE rbObj, Rect* _rect);

  ~Viewport();

  void onRender(sf::RenderTexture& renderTexture) override;

  bool shouldRender() const override;

  int getZIndex() const override;

  void updateSprite();

  void addChild(OnRender* instance);

  void removeChild(OnRender* instance);

  void clear();

  /* --------------------------------------------------- */

  /*
      RGSS Properties
  */

  void bindRubyVars();

  Rect* getRect();
  void setRect(Rect* _rect);

  int getVisible();
  void setVisible(int _visible);

  int getZ();
  void setZ(int _z);

  int getOx();
  void setOx(int _ox);

  int getOy();
  void setOy(int _oy);

  Color* getter_color();
  void setter_color(Color* value);

  Tone* getter_tone();
  void setter_tone(Tone* value);

  /* --------------------------------------------------- */

  /*
      RGSS Methods
  */

  void method_dispose();

  bool method_disposed();

  void method_flash(Color* color, int time);

  void method_update();

  /* --------------------------------------------------- */

  /*
    ⇩⇩⇩ Private ⇩⇩⇩
  */

 private:

  bool visible;
  int z;
  int ox;
  int oy;
  Rect* rect;
  Color* color;
  Tone* tone;

  bool isDisposed;
  bool isAdded;

  void initialize();

  void addToLists();

  void removeFromLists();
};

}  // namespace Eng