#include "engnine/Viewport.hpp"

//

#include "Log.hpp"
#include "integrator/It_Color.hpp"
#include "integrator/It_Tone.hpp"

namespace Eng {

/*

    ⇩⇩⇩   RGSS Properties   ⇩⇩⇩

*/

// Getter rect

Rect* Viewport::getRect()
{
  return rect;
}

// Setter rect

void Viewport::setRect(Rect* _rect)
{
  rect = _rect;
}

// Getter visible

int Viewport::getVisible()
{
  return visible;
}

// Setter visible

void Viewport::setVisible(int _visible)
{
  visible = _visible;
}

/* --------------------------------------------------- */

// Getter z

int Viewport::getZ()
{
  return z;
}

// Setter z

void Viewport::setZ(int _z)
{
  z = _z;
  setInstanceVar("@z", Convert::toRubyNumber(z));
}

/* --------------------------------------------------- */

// Getter ox

int Viewport::getOx()
{
  return ox;
}

// Setter ox

void Viewport::setOx(int _ox)
{
  ox = _ox;
  setInstanceVar("@ox", Convert::toRubyNumber(ox));
}

// Getter oy

int Viewport::getOy()
{
  return oy;
}

// Setter oy

void Viewport::setOy(int val)
{
  oy = val;
  setInstanceVar("@oy", Convert::toRubyNumber(oy));
}

// Getter color

Color* Viewport::getter_color()
{
  return color;
}

// Setter color

void Viewport::setter_color(Color* value)
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

// Getter tone

Tone* Viewport::getter_tone()
{
  return tone;
}

// Setter tone

void Viewport::setter_tone(Tone* value)
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

    ⇩⇩⇩   RGSS Methods   ⇩⇩⇩

*/

/*
  Method dispose
*/

void Viewport::method_dispose()
{
  isDisposed = true;
  removeFromLists();
}

/*
  Method disposed?
*/

bool Viewport::method_disposed()
{
  return isDisposed;
}

/*
  Method flash
*/

void Viewport::method_flash(Color* color, int time)
{
  Log::out() << "Viewport method flash was called but it's not implemented yet.";
}

/*
  Method update
*/

void Viewport::method_update()
{
  // Log::out() << "ViewportViewport method update was called but it's not implemented yet.";
}

}  // namespace Eng