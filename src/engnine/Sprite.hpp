#pragma once

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <algorithm>

#include "base/NumberUtils.hpp"
#include "engnine/Bitmap.hpp"
#include "engnine/Color.hpp"
#include "engnine/Drawable.hpp"
#include "engnine/Engine.hpp"
#include "engnine/EngineBase.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Tone.hpp"
#include "engnine/Viewport.hpp"
#include "engnine/VpRects.hpp"

namespace Eng {

class Sprite : Drawable, public EngineBase {
 public:

  Sprite(Viewport *_viewport = nullptr) :
      spriteColor(255, 255, 255, 255)
  {
    viewport = _viewport;
    bitmap = nullptr;
    visible = true;
    x = 0;
    y = 0;
    z = 0;
    ox = 0;
    oy = 0;
    zoom_x = 1.0;
    zoom_y = 1.0;
    angle = 0;
    mirror = false;
    bush_depth = 0;
    opacity = 255;
    blend_type = 0;
    color = new Color(0, 0, 0, 0);
    tone = new Tone(0, 0, 0, 0);
    isDisposed = false;
    Eng::Engine::getInstance().addDrawable(this);
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
    bitmap = value;
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
    z = value;
    Eng::Engine::getInstance().markZOrderDirty();
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

  int getter_angle()
  {
    return angle;
  }

  // Setter angle

  void setter_angle(int value)
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

  unsigned int getter_opacity()
  {
    return opacity;
  }

  // Setter opacity

  void setter_opacity(unsigned int v)
  {
    Num::clamp(v, 0u, 255u);
    opacity = v;
    spriteColor.a = v;
    dirty = true;
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
    color = value;
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

  void method_flash(Color *color, int time)
  {
    Log::out() << "Sprite method flash was called but it's not implemented yet.";
  }

  void method_update()
  {
    Log::out() << "Sprite method update was called but it's not implemented yet.";
  }

  /* --------------------------------------------------- */

  /*
      Engine Methods
  */

  inline int getZPosition() const override
  {
    return z;
  }

  Viewport *getViewport()
  {
    return viewport;
  }

  sf::Sprite &getSfSprite()
  {
    return sfSprite;
  }

  bool shouldRender()
  {
    return !isDisposed && bitmap != nullptr && !bitmap->disposed();
  }

  void update() override
  {
    applyChanges();
  }

  void draw(sf::RenderTexture &renderTexture) override
  {
    if (!shouldRender()) {
      return;
    }

    float opacity = getter_opacity() / 255.f;

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

    renderTexture.draw(
      sfSprite,
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
      sfSprite.setTexture(
        bitmap->renderTexture.getTexture()
      );

      loadedBitmap = true;
    }

    sfSprite.setColor(spriteColor);

    vp::ViewportRect vp;
    vp::DestinyRect dst;

    sf::Vector2f position(x, y);
    sf::IntRect dstRect(
      ox,
      oy,
      bitmap->width,
      bitmap->height
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

      position.x = dst.x;
      position.y = dst.y;

      dstRect.width = dst.width;
      dstRect.height = dst.height;

      dstRect.left += viewport->getOx();
      dstRect.top += viewport->getOy();

      dstRect.left += dst.left;
      dstRect.top += dst.top;
    }

    sfSprite.setPosition(position);
    sfSprite.setTextureRect(dstRect);

    dirty = false;
  }

  /*
   *   Private
   */

 private:
  Viewport *viewport = nullptr;
  Bitmap *bitmap = nullptr;
  Rect *src_rect = nullptr;
  bool visible;
  double x;
  double y;
  int z;
  double ox;
  double oy;
  double zoom_x;
  double zoom_y;
  int angle;
  bool mirror;
  int bush_depth;
  int opacity;
  int blend_type;
  Color *color = nullptr;
  Tone *tone = nullptr;

  bool dirty = false;
  bool isDisposed = false;
  bool loadedBitmap = false;

  sf::Color spriteColor;
  sf::Sprite sfSprite;
  sf::Texture text;
};

}  // namespace Eng