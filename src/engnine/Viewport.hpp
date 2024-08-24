#pragma once

#include <SFML/Graphics/Rect.hpp>

#include "engnine/Color.hpp"
#include "engnine/EngineBase.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Tone.hpp"
#include "integrator/Convert.hpp"
#include "integrator/It_Color.hpp"
#include "integrator/It_Tone.hpp"

namespace Eng {

class Viewport : public EngineBase {
 public:

  Viewport(Rect* _rect) :
      Viewport(Qnil, _rect) { }

  Viewport(int x, int y, int width, int height) :
      Viewport(Qnil, x, y, width, height) { }

  Viewport(VALUE rbObj, int x, int y, int width, int height);

  Viewport(VALUE rbObj, Rect* _rect);

  ~Viewport();

  void initialize();

  void bindRubyVars();

  /*
    Attr rect
  */
  Rect* getRect() { return rect; }
  void setRect(Rect* _rect)
  {
    rect = _rect;
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
    if (color == value) {
      return;
    }

    color = value;
    if (color == nullptr) {
      setInstanceVar("@color", Qnil);
      return;
    }

    if (!color->hasRbObj()) {
      color->rbObj = It::Color::getRubyObject(color);
    }
    setInstanceVar("@color", color->rbObj);
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
    if (tone == value) {
      return;
    }

    tone = value;
    if (tone == nullptr) {
      setInstanceVar("@tone", Qnil);
      return;
    }

    if (!tone->hasRbObj()) {
      tone->rbObj = It::Tone::getRubyObject(tone);
    }
    setInstanceVar("@tone", tone->rbObj);
  }

  /* --------------------------------------------------- */

  /*
      RGSS Methods
  */

  void method_dispose();

  bool method_disposed();

  void method_flash(Color* color, int time);

  void method_update();

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

  void addToLists();

  void removeFromLists();
};

}  // namespace Eng