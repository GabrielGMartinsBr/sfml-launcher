#include "engnine/Viewport.hpp"

#include <SFML/Graphics/Rect.hpp>

#include "engnine/Color.hpp"
#include "engnine/EngineBase.hpp"
#include "engnine/Lists.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Tone.hpp"
#include "integrator/Convert.hpp"
#include "integrator/It_Color.hpp"
#include "integrator/It_Rect.hpp"
#include "integrator/It_Tone.hpp"

namespace Eng {

Viewport::Viewport(VALUE rbObj, int x, int y, int width, int height) :
    EngineBase(rbObj),
    rect(new Rect(x, y, width, height))
{
  color = new Color(0, 0, 0, 0);
  tone = new Tone(0, 0, 0, 0);

  visible = true;
  z = 0;
  ox = 0;
  oy = 0;

  isDisposed = false;
  isAdded = false;

  bindRubyVars();
  addToLists();
}

Viewport::Viewport(VALUE rbObj, Rect* _rect) :
    EngineBase(rbObj),
    rect(_rect)
{
  color = new Color(0, 0, 0, 0);
  tone = new Tone(0, 0, 0, 0);

  visible = true;
  z = 0;
  ox = 0;
  oy = 0;

  isDisposed = false;
  isAdded = false;

  bindRubyVars();
  addToLists();
}

Viewport::~Viewport()
{
  removeFromLists();
}

void Viewport::bindRubyVars()
{
  if (!hasRbObj()) {
    return;
  }

  if (!rect->hasRbObj()) {
    rect->rbObj = It::Rect::getRubyObject(rect);
    rect->bindRubyVars();
  }
  if (!color->hasRbObj()) {
    color->rbObj = It::Color::getRubyObject(color);
  }
  if (!tone->hasRbObj()) {
    tone->rbObj = It::Tone::getRubyObject(tone);
  }

  setInstanceVar("@visible", Convert::toRubyBool(visible));
  setInstanceVar("@z", Convert::toRubyNumber(z));
  setInstanceVar("@ox", Convert::toRubyNumber(ox));
  setInstanceVar("@oy", Convert::toRubyNumber(oy));
  setInstanceVar("@rect", rect->rbObj);
  setInstanceVar("@color", color->rbObj);
  setInstanceVar("@tone", tone->rbObj);
}

void Viewport::addToLists()
{
  if (isAdded) return;
  Lists::Instance().addViewport(this);
  isAdded = true;
}

void Viewport::removeFromLists()
{
  if (!isAdded) return;
  Lists::Instance().removeViewport(this);
  isAdded = false;
}

}  // namespace Eng