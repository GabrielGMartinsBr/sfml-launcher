#pragma once

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "engnine/Bitmap.h"
#include "engnine/Color.hpp"
#include "engnine/EngineBase.hpp"
#include "engnine/IOnUpdate.h"
#include "engnine/IViewportChild.h"
#include "engnine/Rect.hpp"
#include "engnine/Tone.hpp"
#include "engnine/Viewport.hpp"

namespace Eng {

class Sprite : IOnUpdate, IViewportChild, public EngineBase {
 public:

  Sprite();

  Sprite(VALUE rbObj, Viewport *_viewport = nullptr);

  ~Sprite();

  /*
      Engine Methods
  */

  inline Viewport *getViewport() const override;

  inline int getZIndex() const override;

  bool shouldRender() const override;

  sf::Sprite &getSfSprite();

  void onUpdate() override;

  void onRender(sf::RenderTexture &renderTexture) override;

  void applyChanges();

  /* --------------------------------------------------- */

  void bindRubyProps();

  /* --------------------------------------------------- */

  /*
      RGSS Properties
  */

  // Property bitmap
  Bitmap *getter_bitmap();
  void setter_bitmap(Bitmap *value);

  // Property src_rect
  Rect *getter_src_rect();
  void setter_src_rect(Rect *_src_rect);

  // Property visible
  bool getter_visible();
  void setter_visible(bool value);

  // Property x
  double getter_x();
  void setter_x(double value);

  // Property y
  double getter_y();
  void setter_y(double value);

  // Property z
  int getter_z();
  void setter_z(int value);

  // Property ox
  double getter_ox();
  void setter_ox(double _ox);

  // Property oy
  double getter_oy();
  void setter_oy(double _oy);

  // Property zoom_x
  double getter_zoom_x();
  void setter_zoom_x(double value);

  // Property zoom_y
  double getter_zoom_y();
  void setter_zoom_y(double value);

  // Property angle
  double getter_angle();
  void setter_angle(double value);

  // Property mirror
  bool getter_mirror();
  void setter_mirror(bool value);

  // Property bush_depth
  int getter_bush_depth();
  void setter_bush_depth(int value);

  // Property opacity
  unsigned short getter_opacity();
  void setter_opacity(int v);

  // Property blend_type
  unsigned int getter_blend_type();
  void setter_blend_type(unsigned int v);

  // Property color
  Color *getter_color();
  void setter_color(Color *value);

  // Property tone
  Tone *getter_tone();
  void setter_tone(Tone *value);

  /* --------------------------------------------------- */

  /*
      RGSS Methods
  */

  Viewport *method_viewport();
  void method_dispose();
  bool method_disposed();
  void method_flash(Color *color, int time);
  void method_update();

  /* --------------------------------------------------- */

  /*
   *   Private
   */

 private:
  Viewport *viewport;
  Bitmap *bitmap;
  Rect *src_rect;
  bool visible;
  double x;
  double y;
  int z;
  double ox;
  double oy;
  double zoom_x;
  double zoom_y;
  double angle;
  bool mirror;
  int bush_depth;
  unsigned short opacity;
  int blend_type;
  Color *color;
  Tone *tone;

  bool dirty;
  bool isDisposed;
  bool loadedBitmap;
  bool addedToEngineCycles;

  sf::RenderStates states;
  sf::Glsl::Vec4 spriteColor;
  sf::Glsl::Vec4 flashColor;
  sf::Sprite spr;
  sf::Texture texture;
  sf::Vector2f position;

  bool flashColorIsNil;
  int flashTicks;
  int flashDuration;
  float flashProgress;

  void addToEngineCycles();

  void removeFromEngineCycles();

  void setFlashStart(Color *color, int time);
};

}  // namespace Eng