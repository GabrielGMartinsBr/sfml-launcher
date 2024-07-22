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

  int angle;
  int mirror;
  int bush_depth;
  int blend_type;

  Sprite(Viewport *_viewport = nullptr) :
      spriteColor(255, 255, 255, 255)
  {
    viewport = _viewport;
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
    Eng::Engine::getInstance().addDrawable(this);
  }

  inline int getZPosition() const override
  {
    return z;
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

  sf::Sprite &getSfSprite()
  {
    return sfSprite;
  }

  /*
    Attr x
  */
  int getter_x() { return x; }
  void setter_x(int x)
  {
    this->x = x;
    dirty = true;
  }

  /*
    Attr y
  */
  int getter_y() { return y; }
  void setter_y(int _y)
  {
    y = _y;
    dirty = true;
  }

  /*
    Attr z
  */
  int getter_z() { return z; }
  void setter_z(int _z)
  {
    z = _z;
    Eng::Engine::getInstance().markZOrderDirty();
  }

  /*
    Attr ox
  */
  int getter_ox() { return ox; }
  void setter_ox(int _ox)
  {
    ox = _ox;
    dirty = true;
  }

  /*
    Attr oy
  */
  int getter_oy() { return oy; }
  void setter_oy(int _oy)
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

  /*
    Attr opacity
  */
  unsigned int getter_opacity() { return opacity; }
  void setter_opacity(unsigned int v)
  {
    Num::clamp(v, 0u, 255u);
    opacity = v;
    spriteColor.a = v;
    dirty = true;
  }

  /*
   Attr blend_type
  */
  unsigned int getter_blend_type() { return blend_type; }

  void setter_blend_type(unsigned int v)
  {
    Num::clamp(v, 0u, 2u);
    blend_type = v;
    dirty = true;
  }

  /*
    Attr bitmap
  */

  void setBitmap(Bitmap *_bitmap)
  {
    bitmap = _bitmap;
    dirty = true;
  }

  Bitmap *getBitmap()
  {
    return bitmap;
  }

  /*
   Attr src_rect
  */

  void setSrcRect(Rect *_src_rect)
  {
    src_rect = _src_rect;
    dirty = true;
  }

  Rect *getSrcRect()
  {
    return src_rect;
  }

  void dispose()
  {
    isDisposed = true;
  }

  bool disposed()
  {
    return isDisposed;
  }

  Viewport *getViewport()
  {
    return viewport;
  }

  bool shouldRender()
  {
    return !isDisposed && bitmap != nullptr && !bitmap->disposed();
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
  float x = 0;
  float y = 0;
  int z = 0;
  bool visible = true;
  float ox = 0;
  float oy = 0;
  double zoom_x;
  double zoom_y;
  int opacity = 255;
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