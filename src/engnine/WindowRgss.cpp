#include "aeon/base/AeonIntegrable.h"
#include "engnine/EngineBase.hpp"
#include "engnine/Window.h"
//

#include "NumberUtils.hpp"
#include "engnine/Lists.hpp"
#include "integrator/It_Rect.hpp"
#include "integrator/It_Viewport.hpp"

namespace Eng {

static const uint8_t cursorAniAlpha[] = {
  /* Fade out */
  0xFF, 0xF7, 0xEF, 0xE7, 0xDF, 0xD7, 0xCF, 0xC7,
  0xBF, 0xB7, 0xAF, 0xA7, 0x9F, 0x97, 0x8F, 0x87,
  /* Fade in */
  0x7F, 0x87, 0x8F, 0x97, 0x9F, 0xA7, 0xAF, 0xB7,
  0xBF, 0xC7, 0xCF, 0xD7, 0xDF, 0xE7, 0xEF, 0xF7
};

static const int cursorAniAlphaN = sizeof(cursorAniAlpha);

/*
  Rgss
*/

// Bind props ruby object to instance object

void Window::bindRubyVars()
{
  if (rbObj == Qnil) {
    std::runtime_error("Window doesn't have rbObj defined.");
  }

  VALUE viewportVal = Qnil;
  if (viewport) {
    if (!viewport->hasRbObj()) {
      viewport->rbObj = It::Viewport::getRubyObject(viewport);
      viewport->bindRubyVars();
    }
    viewportVal = viewport->rbObj;
  }

  if (cursor_rect->rbObj == Qnil) {
    cursor_rect->rbObj = It::Rect::createRubyObject(cursor_rect);
    cursor_rect->bindRubyVars();
  }

  rb_iv_set(rbObj, "@viewport", viewportVal);
  rb_iv_set(rbObj, "@cursor_rect", cursor_rect->rbObj);

  rb_iv_set(rbObj, "@windowSkin", Qnil);
  rb_iv_set(rbObj, "@contents", Qnil);

  rb_iv_set(rbObj, "@stretch", Convert::toRubyBool(_stretch));
  rb_iv_set(rbObj, "@visible", Convert::toRubyBool(visible));
  rb_iv_set(rbObj, "@active", Convert::toRubyBool(active));
  rb_iv_set(rbObj, "@pause", Convert::toRubyBool(pause));

  AeonIntegrable::setInstanceVar("@x", bounds.x());
  AeonIntegrable::setInstanceVar("@y", bounds.y());
  AeonIntegrable::setInstanceVar("@width", bounds.width());
  AeonIntegrable::setInstanceVar("@height", bounds.height());
  rb_iv_set(rbObj, "@z", Convert::toRubyNumber(z));
  rb_iv_set(rbObj, "@ox", Convert::toRubyNumber(ox));
  rb_iv_set(rbObj, "@oy", Convert::toRubyNumber(oy));
  rb_iv_set(rbObj, "@opacity", Convert::toRubyNumber(opacity));
  rb_iv_set(rbObj, "@back_opacity", Convert::toRubyNumber(back_opacity));
  rb_iv_set(rbObj, "@contents_opacity", Convert::toRubyNumber(contents_opacity));
}

/*
  Properties
*/

Bitmap *Window::getter_windowskin()
{
  return windowSkin;
}
void Window::setter_windowskin(Bitmap *value)
{
  if (windowSkin == value) {
    return;
  }

  if (value == nullptr) {
    windowSkin = nullptr;
    EngineBase::setInstanceVar("@windowskin", Qnil);
    return;
  }

  value->initRubyObj();
  windowSkin = value;
  EngineBase::setInstanceVar("@windowskin", windowSkin->rbObj);
}

Bitmap *Window::getter_contents()
{
  return contents;
}
void Window::setter_contents(Bitmap *value)
{
  if (contents == value) {
    return;
  }

  if (value == nullptr) {
    contents = nullptr;
    EngineBase::setInstanceVar("@contents", Qnil);
    return;
  }

  value->initRubyObj();
  contents = value;
  EngineBase::setInstanceVar("@contents", contents->rbObj);
}

VALUE Window::getter_stretch()
{
  return Convert::toRubyBool(_stretch);
}

VALUE Window::setter_stretch(VALUE v)
{
  bool value = Convert::toCBool(v);
  if (_stretch == value) return v;
  _stretch = value;
  EngineBase::setInstanceVar("@stretch", v);
  return v;
}

bool Window::stretch() { return _stretch; }
void Window::stretch(bool v)
{
  if (_stretch == v) return;
  _stretch = v;
  EngineBase::setInstanceVar("@stretch", Convert::toRubyBool(v));
}

Rect *Window::getter_cursor_rect() { return cursor_rect; }
void Window::setter_cursor_rect(Rect *v) { cursor_rect = v; }

/*
  Property active
*/

bool Window::getActive() { return active; }
void Window::setActive(bool v)
{
  if (active == v) return;
  active = v;
  EngineBase::setInstanceVar("@active", Convert::toRubyBool(v));
  skinDirty = true;
  contentsDirty = true;
}

VALUE Window::getter_active()
{
  return Convert::toRubyBool(active);
}

VALUE Window::setter_active(VALUE v)
{
  bool value = Convert::toCBool(v);
  if (active == value) return v;
  active = value;
  EngineBase::setInstanceVar("@active", v);
  skinDirty = true;
  contentsDirty = true;
  return v;
}

// --------------

/*
  Property visible
*/

bool Window::getVisible() { return visible; }
void Window::setVisible(bool v)
{
  if (visible == v) return;
  visible = v;
  EngineBase::setInstanceVar("@visible", Convert::toRubyBool(v));
  updateWindowSprite();
}

VALUE Window::getter_visible()
{
  return Convert::toRubyBool(visible);
}

VALUE Window::setter_visible(VALUE v)
{
  bool value = Convert::toCBool(v);
  if (visible == value) return v;
  visible = value;
  EngineBase::setInstanceVar("@visible", v);
  updateWindowSprite();
  return v;
}

// --------------

/*
  Property visible
*/

bool Window::getPause() { return pause; }
void Window::setPause(bool v)
{
  if (pause == v) return;
  pause = v;
  EngineBase::setInstanceVar("@pause", Convert::toRubyBool(v));
}

VALUE Window::getter_pause()
{
  return Convert::toRubyBool(pause);
}

VALUE Window::setter_pause(VALUE v)
{
  bool value = Convert::toCBool(v);
  if (pause == value) return v;
  pause = value;
  EngineBase::setInstanceVar("@pause", v);
  return v;
}

// --------------

int Window::getX()
{
  return bounds.x();
}

void Window::setX(int v)
{
  if (bounds.x() == v) return;
  bounds.x(v);
  AeonIntegrable::setInstanceVar("@x", v);
  updateViewBounds();
  skinDirty = true;
}

int Window::getY()
{
  return bounds.y();
}

void Window::setY(int v)
{
  if (bounds.y() == v) return;
  bounds.y(v);
  AeonIntegrable::setInstanceVar("@y", v);
  updateViewBounds();
  skinDirty = true;
}

/*  Property width */

int Window::getWidth()
{
  return bounds.width();
}

void Window::setWidth(int v)
{
  if (bounds.width() == v) return;
  bounds.width(v);
  AeonIntegrable::setInstanceVar("@width", v);
  updateViewBounds();
  dimensionsDirty = true;
  skinDirty = true;
}

/*  Property height */

int Window::getHeight()
{
  return bounds.height();
}

void Window::setHeight(int v)
{
  if (bounds.height() == v) return;
  bounds.height(v);
  AeonIntegrable::setInstanceVar("@height", v);
  updateViewBounds();
  dimensionsDirty = true;
  skinDirty = true;
}

/*  Property z */

int Window::getZ() { return z; }
void Window::setZ(int v)
{
  if (z == v) return;
  z = v;
  EngineBase::setInstanceVar("@z", Convert::toRubyNumber(v));
  updateWindowSpriteZ();
  Lists::Instance().markZOrderDirty();
}
VALUE Window::setter_z(VALUE v)
{
  int value = Convert::toCInt2(v);
  if (z == value) return v;
  z = value;
  EngineBase::setInstanceVar("@z", v);
  updateWindowSpriteZ();
  Lists::Instance().markZOrderDirty();
  return v;
}

/*  Property ox */

int Window::getter_ox() { return ox; }
void Window::setter_ox(int v)
{
  if (ox == v) return;
  ox = v;
  EngineBase::setInstanceVar("@ox", Convert::toRubyNumber(v));
  contentsDirty = true;
}
VALUE Window::setter_ox(VALUE v)
{
  int value = Convert::toCInt2(v);
  if (ox == value) return v;
  ox = value;
  EngineBase::setInstanceVar("@ox", v);
  contentsDirty = true;
  return v;
}

/*  Property oy */

int Window::getter_oy() { return oy; }
void Window::setter_oy(int v)
{
  if (oy == v) return;
  oy = v;
  EngineBase::setInstanceVar("@oy", Convert::toRubyNumber(v));
  contentsDirty = true;
}
VALUE Window::setter_oy(VALUE v)
{
  int value = Convert::toCInt2(v);
  if (oy == value) return v;
  oy = value;
  EngineBase::setInstanceVar("@oy", v);
  contentsDirty = true;
  return v;
}

/*  Property opacity */

int Window::getter_opacity() { return opacity; }
void Window::setter_opacity(int v)
{
  int value = Num::clamp(v, 0, 255);
  if (opacity == value) return;
  opacity = value;
  EngineBase::setInstanceVar("@opacity", Convert::toRubyNumber(value));
  opacityDirty = true;
}
VALUE Window::setter_opacity(VALUE v)
{
  int value = Convert::toCInt2(v);
  value = Num::clamp(value, 0, 255);
  if (opacity == value) return v;
  v = Convert::toRubyNumber(value);
  opacity = value;
  EngineBase::setInstanceVar("@opacity", v);
  opacityDirty = true;
  return v;
}

/*  Property back_opacity */

int Window::getter_back_opacity() { return back_opacity; }
void Window::setter_back_opacity(int v)
{
  int value = Num::clamp(v, 0, 255);
  if (back_opacity == value) return;
  back_opacity = value;
  EngineBase::setInstanceVar("@back_opacity", Convert::toRubyNumber(value));
  opacityDirty = true;
}
VALUE Window::setter_back_opacity(VALUE v)
{
  int value = Convert::toCInt2(v);
  value = Num::clamp(value, 0, 255);
  if (back_opacity == value) return v;
  v = Convert::toRubyNumber(value);
  back_opacity = value;
  EngineBase::setInstanceVar("@back_opacity", v);
  opacityDirty = true;
  return v;
}

/*  Property contents_opacity */

int Window::getter_contents_opacity() { return contents_opacity; }
void Window::setter_contents_opacity(int v)
{
  int value = Num::clamp(v, 0, 255);
  if (contents_opacity == value) return;
  contents_opacity = value;
  EngineBase::setInstanceVar("@contents_opacity", Convert::toRubyNumber(value));
  opacityDirty = true;
}
VALUE Window::setter_contents_opacity(VALUE v)
{
  int value = Convert::toCInt2(v);
  value = Num::clamp(value, 0, 255);
  if (contents_opacity == value) return v;
  v = Convert::toRubyNumber(value);
  contents_opacity = value;
  EngineBase::setInstanceVar("@contents_opacity", v);
  opacityDirty = true;
  return v;
}

/*
  Methods
*/

// Method viewport

Viewport *Window::method_viewport()
{
  return viewport;
}

// Method dispose

void Window::dispose()
{
  isDisposed = true;
  frame.dispose();
  contentsSprite.dispose();
  cursor.dispose();
  removeFromEngineCycles();
}

// Method disposed?

bool Window::disposed()
{
  return isDisposed;
}

void Window::method_update()
{
  if (isDisposed || contents == nullptr || contents->disposed()) {
    return;
  }
  cursorAniAlphaId = (cursorAniAlphaId + 1) % cursorAniAlphaN;
  cursorSprite.setOpacity(cursorAniAlpha[cursorAniAlphaId]);
  contentsDirty = contents->dirty;
}

}  // namespace Eng