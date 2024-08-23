#pragma once

#include <SFML/Graphics/Rect.hpp>

#include "Log.hpp"
#include "engnine/Color.hpp"
#include "engnine/EngineBase.hpp"
#include "engnine/RGSSViewport.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Tone.hpp"
#include "integrator/Convert.hpp"
#include "integrator/It_Rect.hpp"

namespace Eng {

class Viewport : public EngineBase {
 public:

  Viewport(Rect* _rect) :
      Viewport(Qnil, _rect) { }

  Viewport(int x, int y, int width, int height) :
      Viewport(Qnil, x, y, width, height) { }

  Viewport(VALUE rbObj, int x, int y, int width, int height) :
      EngineBase(rbObj),
      rect(x, y, width, height),
      rgssVp(x, y, width, height)
  {
    color = new Color(0, 0, 0, 0);
    tone = new Tone(0, 0, 0, 0);
    isDisposed = false;

    visible = true;
    z = 0;
    ox = 0;
    oy = 0;

    bindRubyVars();
  }

  Viewport(VALUE rbObj, Rect* _rect) :
      EngineBase(rbObj),
      rect(_rect),
      rgssVp(rect)
  {
    color = new Color(0, 0, 0, 0);
    tone = new Tone(0, 0, 0, 0);
    isDisposed = false;

    visible = true;
    z = 0;
    ox = 0;
    oy = 0;

    bindRubyVars();
  }

  void bindRubyVars()
  {
    if (!hasRbObj()) {
      return;
    }

    if (!rect.hasRbObj()) {
      rect.rbObj = It::Rect::getRubyObject(&rect);
      rect.bindRubyVars();
    }

    setInstanceVar("@visible", Convert::toRubyBool(visible));
    setInstanceVar("@z", Convert::toRubyNumber(z));
    setInstanceVar("@ox", Convert::toRubyNumber(ox));
    setInstanceVar("@oy", Convert::toRubyNumber(oy));
    setInstanceVar("@rect", rect.rbObj);
  }

  RGSS::Viewport& getRgssViewport()
  {
    return rgssVp;
  }

  /*
    Attr rect
  */
  Rect* getRect() { return &rect; }
  void setRect(Rect* _rect)
  {
    rect = *_rect;
  }

  /*
    Attr visible
  */
  int getVisible() { return visible; }
  void setVisible(int _visible)
  {
    visible = _visible;
  }

  /*
    Attr z
  */
  int getZ() { return z; }
  void setZ(int _z)
  {
    z = _z;
    setInstanceVar("@z", Convert::toRubyNumber(z));
  }

  /*
    Attr ox
  */
  int getOx() { return ox; }
  void setOx(int _ox)
  {
    ox = _ox;
    setInstanceVar("@ox", Convert::toRubyNumber(ox));
  }

  /*
    Attr oy
  */
  int getOy() { return oy; }
  void setOy(int _oy)
  {
    oy = _oy;
    setInstanceVar("@oy", Convert::toRubyNumber(oy));
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

  /*
      RGSS Methods
  */

  void method_dispose()
  {
    isDisposed = true;
  }

  bool method_disposed()
  {
    return isDisposed;
  }

  void method_flash(Color* color, int time)
  {
    Log::out() << "Viewport method flash was called but it's not implemented yet.";
  }

  void method_update()
  {
    // Log::out() << "ViewportViewport method update was called but it's not implemented yet.";
  }

 private:
  Rect rect;
  bool visible;
  int z;
  int ox;
  int oy;
  Color* color;
  Tone* tone;

  bool isDisposed;

  RGSS::Viewport rgssVp;
};

}  // namespace Eng