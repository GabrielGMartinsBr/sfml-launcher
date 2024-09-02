#include "Sprite.h"
//

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <stdexcept>

#include "Log.hpp"
#include "base/NumberUtils.hpp"
#include "engnine/Bitmap.h"
#include "engnine/Color.hpp"
#include "engnine/Lists.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Tone.hpp"
#include "engnine/Viewport.hpp"
#include "integrator/Convert.hpp"
#include "integrator/It_Bitmap.hpp"
#include "integrator/It_Color.hpp"
#include "integrator/It_Rect.hpp"
#include "integrator/It_Tone.hpp"
#include "integrator/It_Viewport.hpp"

namespace Eng {

void Sprite::bindRubyProps()
{
  if (rbObj == Qnil) {
    std::runtime_error("Sprite doesn't have rbObj defined.");
  }

  VALUE vp = Qnil;
  if (viewport != nullptr) {
    if (!viewport->hasRbObj()) {
      viewport->rbObj = It::Viewport::createRubyObject(viewport);
      viewport->bindRubyVars();
    }
    vp = viewport->rbObj;
  }

  if (src_rect->rbObj == Qnil) {
    src_rect->rbObj = It::Rect::createRubyObject(src_rect);
  }
  if (color->rbObj == Qnil) {
    color->rbObj = It::Color::createRubyObject(color);
  }
  if (tone->rbObj == Qnil) {
    tone->rbObj = It::Tone::createRubyObject(tone);
  }

  rb_iv_set(rbObj, "@viewport", vp);
  rb_iv_set(rbObj, "@src_rect", src_rect->rbObj);
  rb_iv_set(rbObj, "@color", color->rbObj);
  rb_iv_set(rbObj, "@tone", tone->rbObj);
  rb_iv_set(rbObj, "@z", Convert::toRubyNumber(z));
  rb_iv_set(rbObj, "@opacity", Convert::toRubyNumber(opacity));
}

/* --------------------------------------------------- */

// Getter bitmap

Bitmap *Sprite::getter_bitmap()
{
  return bitmap;
}

// Setter bitmap

void Sprite::setter_bitmap(Bitmap *value)
{
  if (bitmap == value) {
    return;
  }

  if (value->rbObj == Qnil) {
    value->rbObj = It::Bitmap::createRubyObject(value);
  }

  bitmap = value;
  rb_iv_set(rbObj, "@bitmap", bitmap->rbObj);

  src_rect->width.set(bitmap->getter_width());
  src_rect->height.set(bitmap->getter_height());

  dirty = true;
}

/* --------------------------------------------------- */

// Getter src_rect

Rect *Sprite::getter_src_rect()
{
  return src_rect;
}

// Setter src_rect

void Sprite::setter_src_rect(Rect *_src_rect)
{
  // Log::out() << "(((settings src rect)))";
  src_rect = _src_rect;
  dirty = true;
}

/* --------------------------------------------------- */

// Getter visible

bool Sprite::getter_visible()
{
  return visible;
}

// Setter visible

void Sprite::setter_visible(bool value)
{
  visible = value;
}

/* --------------------------------------------------- */

// Getter x

double Sprite::getter_x()
{
  return x;
}

// Setter x

void Sprite::setter_x(double value)
{
  x = value;
  dirty = true;
}

/* --------------------------------------------------- */

// Getter y

double Sprite::getter_y()
{
  return y;
}

// Setter y

void Sprite::setter_y(double value)
{
  y = value;
  dirty = true;
}

/* --------------------------------------------------- */

// Getter z

int Sprite::getter_z()
{
  return z;
}

// Setter z

void Sprite::setter_z(int value)
{
  if (z != value) {
    z = value;
    setInstanceVar("@z", Convert::toRubyNumber(value));
    Lists::Instance().markZOrderDirty();
  }
}

/* --------------------------------------------------- */

// Getter ox

double Sprite::getter_ox()
{
  return ox;
}

// Setter ox

void Sprite::setter_ox(double _ox)
{
  ox = _ox;
  dirty = true;
}

/* --------------------------------------------------- */

// Getter oy

double Sprite::getter_oy()
{
  return oy;
}

// Setter oy

void Sprite::setter_oy(double _oy)
{
  oy = _oy;
  dirty = true;
}

/* --------------------------------------------------- */

// Getter zoom_x

double Sprite::getter_zoom_x()
{
  return zoom_x;
}

// Setter zoom_x

void Sprite::setter_zoom_x(double value)
{
  zoom_x = value;
  dirty = true;
}

/* --------------------------------------------------- */

// Getter zoom_y

double Sprite::getter_zoom_y()
{
  return zoom_y;
}

// Setter zoom_y

void Sprite::setter_zoom_y(double value)
{
  zoom_y = value;
  dirty = true;
}

/* --------------------------------------------------- */

// Getter angle

double Sprite::getter_angle()
{
  return angle;
}

// Setter angle

void Sprite::setter_angle(double value)
{
  angle = value;
  dirty = true;
}

/* --------------------------------------------------- */

// Getter mirror

bool Sprite::getter_mirror()
{
  return mirror;
}

// Setter mirror

void Sprite::setter_mirror(bool value)
{
  mirror = value;
  dirty = true;
}

/* --------------------------------------------------- */

// Getter bush_depth

int Sprite::getter_bush_depth()
{
  return bush_depth;
}

// Setter bush_depth

void Sprite::setter_bush_depth(int value)
{
  bush_depth = value;
  dirty = true;
}

/* --------------------------------------------------- */

// Getter opacity

unsigned short Sprite::getter_opacity()
{
  return opacity;
}

// Setter opacity

void Sprite::setter_opacity(int v)
{
  int value = Num::clamp(v, 0, 255);
  if (opacity == value) {
    return;
  }
  opacity = value;
  rb_iv_set(rbObj, "@opacity", Convert::toRubyNumber(opacity));
  dirty = true;
}

/* --------------------------------------------------- */

// Getter blend_type

unsigned int Sprite::getter_blend_type()
{
  return blend_type;
}

// Setter blend_type

void Sprite::setter_blend_type(unsigned int v)
{
  Num::clamp(v, 0u, 2u);
  blend_type = v;
  dirty = true;
}

/* --------------------------------------------------- */

// Getter color

Color *Sprite::getter_color()
{
  return color;
}

// Setter color

void Sprite::setter_color(Color *value)
{
  if (color == value) {
    return;
  }

  if (value->rbObj == Qnil) {
    value->rbObj = It::Color::createRubyObject(value);
  }

  color = value;
  rb_iv_set(rbObj, "@color", color->rbObj);
}

/* --------------------------------------------------- */

// Getter tone

Tone *Sprite::getter_tone()
{
  return tone;
}

// Setter tone

void Sprite::setter_tone(Tone *value)
{
  if (tone == value) {
    return;
  }

  if (value->rbObj == Qnil) {
    value->rbObj = It::Tone::createRubyObject(value);
  }

  tone = value;
  rb_iv_set(rbObj, "@tone", tone->rbObj);
}

/* --------------------------------------------------- */

/*
    RGSS Methods
*/

Viewport *Sprite::method_viewport()
{
  return viewport;
}

void Sprite::method_dispose()
{
  isDisposed = true;
  removeFromEngineCycles();
}

bool Sprite::method_disposed()
{
  return isDisposed;
}

void Sprite::method_flash(Color *color, int time)
{
  Log::out() << "Sprite method flash was called but it's not implemented yet.";
}

void Sprite::method_update()
{
  // Log::out() << "Sprite method update was called but it's not implemented yet.";
}

}  // namespace Eng