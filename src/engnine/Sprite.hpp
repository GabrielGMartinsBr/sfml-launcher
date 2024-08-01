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
#include <algorithm>
#include <stdexcept>

#include "Log.hpp"
#include "base/NumberUtils.hpp"
#include "engnine/Bitmap.h"
#include "engnine/Color.hpp"
#include "engnine/Engine.h"
#include "engnine/EngineBase.hpp"
#include "engnine/OnRender.h"
#include "engnine/OnUpdate.h"
#include "engnine/Rect.hpp"
#include "engnine/Tone.hpp"
#include "engnine/Viewport.hpp"
#include "engnine/VpRects.hpp"
#include "integrator/It_Bitmap.hpp"
#include "integrator/It_Color.hpp"
#include "integrator/It_Rect.hpp"
#include "integrator/It_Tone.hpp"
#include "integrator/It_Viewport.hpp"

namespace Eng {

class Sprite : OnUpdate, OnRender, public EngineBase {
 public:

  // Constructor

  Sprite(VALUE rbObj, Viewport *_viewport = nullptr) :
      EngineBase(rbObj),
      spriteColor(255, 255, 255, 255),
      position(0, 0)
  {
    viewport = _viewport;
    bitmap = nullptr;
    src_rect = new Rect(0, 0, 0, 0);
    visible = true;
    x = 0;
    y = 0;
    z = 0;
    ox = 0;
    oy = 0;
    zoom_x = 1.0;
    zoom_y = 1.0;
    angle = 0.0;
    mirror = false;
    bush_depth = 0;
    opacity = 255;
    blend_type = 0;
    color = new Color(0, 0, 0, 0);
    tone = new Tone(0, 0, 0, 0);

    dirty = false;
    isDisposed = false;
    loadedBitmap = false;
    addedToEngineCycles = false;

    if (rbObj != Qnil) {
      bindRubyProps();
    }

    addToEngineCycles();
  }

  Sprite(Viewport *_viewport = nullptr) :
      Sprite(Qnil, _viewport) { }

  ~Sprite()
  {
    Log::out() << "Destructor sprite";
    removeFromEngineCycles();
  }

  /*
      Engine Methods
  */

  inline int getZIndex() const override
  {
    return z;
  }

  bool shouldRender() const override
  {
    return !isDisposed && visible && bitmap != nullptr && !bitmap->disposed();
  }

  sf::Sprite &getSfSprite()
  {
    return spr;
  }

  void onUpdate() override
  {
    if (!dirty || !bitmap) {
      return;
    }

    if (!loadedBitmap || bitmap->dirty) {
      spr.setTexture(
        bitmap->renderTexture.getTexture()
      );
      loadedBitmap = true;
      bitmap->dirty = false;
    }

    position.x = x - ox;
    position.y = y - oy;

    sf::IntRect srcRect = src_rect->sfRect();

    spr.setPosition(position);

    spr.setTextureRect(srcRect);
  }

  void onRender(sf::RenderTexture &renderTexture) override
  {
    float opacity = getter_opacity() / 255.f;

    renderTexture.draw(spr);

    return;

    sf::RenderStates state;
    // state.blendMode = sf::BlendNone;

    if (getter_blend_type() == 2) {
      Engine::getInstance().blendShaders.sprInvertShader.setUniform("opacity", opacity);
      state.shader = &Engine::getInstance().blendShaders.sprInvertShader;
      state.blendMode = sf::BlendMultiply;
    } else {
      Engine::getInstance().blendShaders.sprNormalShader.setUniform("opacity", opacity);
      state.shader = &Engine::getInstance().blendShaders.sprNormalShader;
      state.blendMode = sf::BlendAlpha;
    }
    // Engine::getInstance().blendShaders.sprInvertShader.setUniform("opacity", opacity);
    // state.shader = &Engine::getInstance().blendShaders.sprInvertShader;
    // state.blendMode = sf::BlendMultiply;
    // state.blendMode = sf::BlendAlpha;

    // Log::out() << " ---> Position: " << sfSprite.getPosition();

    renderTexture.draw(
      spr,
      state
    );
    renderTexture.display();
  }

  void applyChanges()
  {
    if (!dirty || !bitmap) {
      return;
    }

    if (!loadedBitmap || bitmap->dirty) {
      spr.setTexture(
        bitmap->renderTexture.getTexture()
      );

      loadedBitmap = true;
    }

    spr.setColor(spriteColor);

    vp::ViewportRect vp;
    vp::DestinyRect dst;

    sf::Vector2f position(x, y);
    sf::IntRect dstRect(
      ox,
      oy,
      bitmap->getter_width(),
      bitmap->getter_height()
    );

    if (src_rect) {
      dstRect.left = src_rect->getter_x() + ox;
      dstRect.top = src_rect->getter_y() + oy;
      dstRect.width = src_rect->getter_width();
      dstRect.height = src_rect->getter_height();
    }

    if (viewport) {
      auto &vpRect = *viewport->getRect();

      // ---
      vp.x = vpRect.getter_x();
      vp.y = vpRect.getter_y();
      vp.width = vpRect.getter_width();
      vp.height = vpRect.getter_height();
      dst.x = vp.x;
      dst.y = vp.y;

      int dstX = vpRect.getter_x() + x;
      int dstY = vpRect.getter_y() + y;

      if (dstX > vp.endX() || dstY > vp.endY()) {
        return;
      }

      dst.width = vp.width;
      dst.height = vp.height;

      if (x > 0) {
        dst.x += x;
        dst.width -= x;
      } else if (x < 0) {
        dst.left -= x;
      }
      if (y > 0) {
        dst.y += y;
        dst.height -= y;
      }
      if (y < 0) {
        dst.top -= y;
      }

      dst.width = std::min(dst.width, dstRect.width);
      dst.height = std::min(dst.height, dstRect.height);

      position.x = dst.x + ox;
      position.y = dst.y + oy;

      dstRect.width = dst.width;
      dstRect.height = dst.height;

      dstRect.left += viewport->getOx();
      dstRect.top += viewport->getOy();

      dstRect.left += dst.left;
      dstRect.top += dst.top;
    }

    // dstRect.left += 16;
    // dstRect.top += 16;

    spr.setPosition(position);
    spr.setTextureRect(dstRect);

    dirty = false;
  }

  /* --------------------------------------------------- */

  /*
    Bind ruby props
  */

  void bindRubyProps()
  {
    if (rbObj == Qnil) {
      std::runtime_error("Sprite doesn't have rbObj defined.");
    }

    if (viewport != nullptr && viewport->rbObj == Qnil) {
      viewport->rbObj = It::Viewport::createRubyObject(viewport);
      rb_iv_set(rbObj, "@viewport", viewport->rbObj);
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

    rb_iv_set(rbObj, "@src_rect", src_rect->rbObj);
    rb_iv_set(rbObj, "@color", color->rbObj);
    rb_iv_set(rbObj, "@tone", tone->rbObj);
  }

  /* --------------------------------------------------- */

  // Getter bitmap

  Bitmap *getter_bitmap()
  {
    return bitmap;
  }

  // Setter bitmap

  void setter_bitmap(Bitmap *value)
  {
    if (bitmap == value) {
      return;
    }

    if (value->rbObj == Qnil) {
      value->rbObj = It::Bitmap::createRubyObject(value);
    }

    bitmap = value;
    rb_iv_set(rbObj, "@bitmap", bitmap->rbObj);

    src_rect->setter_width(bitmap->getter_width());
    src_rect->setter_height(bitmap->getter_height());

    dirty = true;
  }

  /* --------------------------------------------------- */

  // Getter src_rect

  Rect *getter_src_rect()
  {
    return src_rect;
  }

  // Setter src_rect

  void setter_src_rect(Rect *_src_rect)
  {
    // Log::out() << "(((settings src rect)))";
    src_rect = _src_rect;
    dirty = true;
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

  // Getter x

  double getter_x()
  {
    return x;
  }

  // Setter x

  void setter_x(double value)
  {
    x = value;
    dirty = true;
  }

  /* --------------------------------------------------- */

  // Getter y

  double getter_y()
  {
    return y;
  }

  // Setter y

  void setter_y(double value)
  {
    y = value;
    dirty = true;
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
    if (z != value) {
      z = value;
      Eng::Engine::getInstance().markZOrderDirty();
    }
  }

  /* --------------------------------------------------- */

  // Getter ox

  double getter_ox()
  {
    return ox;
  }

  // Setter ox

  void setter_ox(double _ox)
  {
    ox = _ox;
    dirty = true;
  }

  /* --------------------------------------------------- */

  // Getter oy

  double getter_oy()
  {
    return oy;
  }

  // Setter oy

  void setter_oy(double _oy)
  {
    oy = _oy;
    dirty = true;
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
    dirty = true;
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
    dirty = true;
  }

  /* --------------------------------------------------- */

  // Getter angle

  double getter_angle()
  {
    return angle;
  }

  // Setter angle

  void setter_angle(double value)
  {
    angle = value;
    dirty = true;
  }

  /* --------------------------------------------------- */

  // Getter mirror

  bool getter_mirror()
  {
    return mirror;
  }

  // Setter mirror

  void setter_mirror(bool value)
  {
    mirror = value;
    dirty = true;
  }

  /* --------------------------------------------------- */

  // Getter bush_depth

  int getter_bush_depth()
  {
    return bush_depth;
  }

  // Setter bush_depth

  void setter_bush_depth(int value)
  {
    bush_depth = value;
    dirty = true;
  }

  /* --------------------------------------------------- */

  // Getter opacity

  unsigned short getter_opacity()
  {
    return opacity;
  }

  // Setter opacity

  void setter_opacity(int v)
  {
    int value = Num::clamp(v, 0, 255);
    if (opacity != value) {
      opacity = value;
      dirty = true;
    }
  }

  /* --------------------------------------------------- */

  // Getter blend_type

  unsigned int getter_blend_type()
  {
    return blend_type;
  }

  // Setter blend_type

  void setter_blend_type(unsigned int v)
  {
    Num::clamp(v, 0u, 2u);
    blend_type = v;
    dirty = true;
  }

  /* --------------------------------------------------- */

  // Getter color

  Color *getter_color()
  {
    return color;
  }

  // Setter color

  void setter_color(Color *value)
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

  Tone *getter_tone()
  {
    return tone;
  }

  // Setter tone

  void setter_tone(Tone *value)
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

  Viewport *method_viewport()
  {
    return viewport;
  }

  void method_dispose()
  {
    isDisposed = true;
    removeFromEngineCycles();
  }

  bool method_disposed()
  {
    return isDisposed;
  }

  void method_flash(Color *color, int time)
  {
    Log::out() << "Sprite method flash was called but it's not implemented yet.";
  }

  void method_update()
  {
    // Log::out() << "Sprite method update was called but it's not implemented yet.";
  }

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

  sf::Color spriteColor;
  sf::Sprite spr;
  sf::Texture texture;
  sf::Vector2f position;

  void addToEngineCycles()
  {
    if (addedToEngineCycles) {
      return;
    }
    Eng::Engine::getInstance().addToUpdateList(this);
    Eng::Engine::getInstance().addToRenderList(this);
    addedToEngineCycles = true;
  }

  void removeFromEngineCycles()
  {
    if (!addedToEngineCycles) {
      return;
    }
    Eng::Engine::getInstance().removeFromUpdateList(this);
    Eng::Engine::getInstance().removeFromRenderList(this);
    addedToEngineCycles = false;
  }
};

}  // namespace Eng