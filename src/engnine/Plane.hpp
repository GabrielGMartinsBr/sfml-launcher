#pragma once

#include "engnine/Bitmap.h"
#include "engnine/EngineBase.hpp"
#include "engnine/Tone.hpp"
#include "engnine/Viewport.hpp"
#include "integrator/It_Bitmap.hpp"
#include "integrator/It_Color.hpp"
#include "integrator/It_Tone.hpp"
#include "integrator/It_Viewport.hpp"

namespace Eng {

class Plane : public EngineBase {
 public:
  Plane(Viewport* vp = nullptr) :
      Plane(Qnil, vp) { }
  Plane(VALUE rbObj, Viewport* vp = nullptr) :
      EngineBase(rbObj)
  {
    viewport = vp;
    bitmap = nullptr;
    visible = true;
    z = 0;
    ox = 0;
    oy = 0;
    zoom_x = 1.0;
    zoom_y = 1.0;
    opacity = 255;
    blend_type = 0;
    color = new Color(0, 0, 0, 0);
    tone = new Tone(0, 0, 0, 0);
    isDisposed = false;

    if (rbObj != Qnil) {
      bindRubyProps();
    }
  }

  /* --------------------------------------------------- */

  /*
    Bind ruby props
  */

  void bindRubyProps()
  {
    if (rbObj == Qnil) {
      std::runtime_error("Plane doesn't have rbObj defined.");
    }

    if (viewport != nullptr && viewport->rbObj == Qnil) {
      viewport->rbObj = It::Viewport::createRubyObject(viewport);
      rb_iv_set(rbObj, "@viewport", viewport->rbObj);
    }

    if (color->rbObj == Qnil) {
      color->rbObj = It::Color::createRubyObject(color);
    }
    if (tone->rbObj == Qnil) {
      tone->rbObj = It::Tone::createRubyObject(tone);
    }

    rb_iv_set(rbObj, "@color", color->rbObj);
    rb_iv_set(rbObj, "@tone", tone->rbObj);
  }

  /* --------------------------------------------------- */

  // Getter bitmap

  Bitmap* getter_bitmap()
  {
    return bitmap;
  }

  // Setter bitmap

  void setter_bitmap(Bitmap* value)
  {
    if (bitmap == value) {
      return;
    }

    if (value->rbObj == Qnil) {
      value->rbObj = It::Bitmap::createRubyObject(value);
    }

    bitmap = value;
    rb_iv_set(rbObj, "@bitmap", bitmap->rbObj);
  }

  /* --------------------------------------------------- */

  // Getter visible

  bool getter_visible()
  {
    return visible;
  }

  // Setter visible

  void setter_visible(bool value)
  {
    visible = value;
  }

  /* --------------------------------------------------- */

  // Getter z

  int getter_z()
  {
    return z;
  }

  // Setter z

  void setter_z(int value)
  {
    z = value;
  }

  /* --------------------------------------------------- */

  // Getter ox

  int getter_ox()
  {
    return ox;
  }

  // Setter ox

  void setter_ox(int value)
  {
    ox = value;
  }

  /* --------------------------------------------------- */

  // Getter oy

  int getter_oy()
  {
    return oy;
  }

  // Setter oy

  void setter_oy(int value)
  {
    oy = value;
  }

  /* --------------------------------------------------- */

  // Getter zoom_x

  double getter_zoom_x()
  {
    return zoom_x;
  }

  // Setter zoom_x

  void setter_zoom_x(double value)
  {
    zoom_x = value;
  }

  /* --------------------------------------------------- */

  // Getter zoom_y

  double getter_zoom_y()
  {
    return zoom_y;
  }

  // Setter zoom_y

  void setter_zoom_y(double value)
  {
    zoom_y = value;
  }

  /* --------------------------------------------------- */

  // Getter opacity

  int getter_opacity()
  {
    return opacity;
  }

  // Setter opacity

  void setter_opacity(int value)
  {
    opacity = value;
  }

  /* --------------------------------------------------- */

  // Getter blend_type

  int getter_blend_type()
  {
    return blend_type;
  }

  // Setter blend_type

  void setter_blend_type(int value)
  {
    blend_type = value;
  }

  /* --------------------------------------------------- */

  // Getter color

  Color* getter_color()
  {
    return color;
  }

  // Setter color

  void setter_color(Color* value)
  {
    color = value;
  }

  /* --------------------------------------------------- */

  // Getter tone

  Tone* getter_tone()
  {
    return tone;
  }

  // Setter tone

  void setter_tone(Tone* value)
  {
    tone = value;
  }

  /* --------------------------------------------------- */

  // Method dispose

  void method_dispose()
  {
    isDisposed = true;
  }

  // Method disposed

  bool method_disposed()
  {
    return isDisposed;
  }

  // Method viewport

  Viewport* method_viewport()
  {
    return viewport;
  }

 private:
  Viewport* viewport;
  Bitmap* bitmap;
  bool visible;
  int z;
  int ox;
  int oy;
  double zoom_x;
  double zoom_y;
  int opacity;
  int blend_type;
  Color* color;
  Tone* tone;

  bool isDisposed;
};

}  // namespace Eng