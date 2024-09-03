#include "engnine/Sprite.h"

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

#include "engnine/Bitmap.h"
#include "engnine/Color.hpp"
#include "engnine/EngineBase.hpp"
#include "engnine/Lists.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Shaders.h"
#include "engnine/Tone.hpp"
#include "engnine/Viewport.hpp"
#include "engnine/VpRects.hpp"

namespace Eng {

Sprite::Sprite() :
    Sprite(Qnil) { }

Sprite::Sprite(VALUE rbObj, Viewport *_viewport) :
    EngineBase(rbObj),
    spriteColor(0, 0, 0, 0),
    flashColor(0, 0, 0, 0),
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

  flashColorIsNil = false;
  flashTicks = 0;
  flashDuration = 0;
  flashProgress = 0;

  if (rbObj != Qnil) {
    bindRubyProps();
  }

  addToEngineCycles();
}

Sprite::~Sprite()
{
  removeFromEngineCycles();
}

/*
    Engine Methods
*/

inline Viewport *Sprite::getViewport() const
{
  return viewport;
}

inline int Sprite::getZIndex() const
{
  return z;
}

bool Sprite::shouldRender() const
{
  return !isDisposed && visible && bitmap != nullptr && !bitmap->disposed();
}

sf::Sprite &Sprite::getSfSprite()
{
  return spr;
}

void Sprite::onUpdate()
{
  if (!bitmap || (!dirty && bitmap && !bitmap->dirty)) {
    return;
  }

  if (color->isDirty()) {
    spriteColor.x = color->red / 255;
    spriteColor.y = color->green / 255;
    spriteColor.z = color->blue / 255;
    spriteColor.w = color->alpha / 255;
    color->markAsClean();
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

void Sprite::onRender(sf::RenderTexture &renderTexture)
{
  states.blendMode = sf::BlendAlpha;

  if (flashTicks == 0) {
    Shaders::Instance().spriteColor->setUniform("color", spriteColor);
    Shaders::Instance().spriteColor->setUniform("opacity", opacity / 255.0f);
    states.shader = Shaders::Instance().spriteColor.get();
    renderTexture.draw(spr, states);
    return;
  }

  if (flashColorIsNil) return;

  flashProgress = static_cast<float>(flashTicks) / flashDuration;
  Shaders::Instance().spriteFlash->setUniform("color", spriteColor);
  Shaders::Instance().spriteFlash->setUniform("opacity", opacity / 255.0f);
  Shaders::Instance().spriteFlash->setUniform("flash", flashColor);
  Shaders::Instance().spriteFlash->setUniform("progress", flashProgress);
  states.shader = Shaders::Instance().spriteFlash.get();
  renderTexture.draw(spr, states);

  return;
  // sf::RenderStates state;
  // state.blendMode = sf::BlendNone;

  // if (getter_blend_type() == 2) {
  //   Engine::getInstance().blendShaders.sprInvertShader.setUniform("opacity", opacity);
  //   state.shader = &Engine::getInstance().blendShaders.sprInvertShader;
  //   state.blendMode = sf::BlendMultiply;
  // } else {
  //   Engine::getInstance().blendShaders.sprNormalShader.setUniform("opacity", opacity);
  //   state.shader = &Engine::getInstance().blendShaders.sprNormalShader;
  //   state.blendMode = sf::BlendAlpha;
  // }
  // Engine::getInstance().blendShaders.sprInvertShader.setUniform("opacity", opacity);
  // state.shader = &Engine::getInstance().blendShaders.sprInvertShader;
  // state.blendMode = sf::BlendMultiply;
  // state.blendMode = sf::BlendAlpha;

  // renderTexture.draw(
  //   spr,
  //   state
  // );
  // renderTexture.display();
}

void Sprite::applyChanges()
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

  // spr.setColor(spriteColor);

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
    dstRect.left = src_rect->x.get() + ox;
    dstRect.top = src_rect->y.get() + oy;
    dstRect.width = src_rect->width.get();
    dstRect.height = src_rect->height.get();
  }

  if (viewport) {
    auto &vpRect = *viewport->getRect();

    // ---
    vp.x = vpRect.x.get();
    vp.y = vpRect.y.get();
    vp.width = vpRect.width.get();
    vp.height = vpRect.height.get();
    dst.x = vp.x;
    dst.y = vp.y;

    int dstX = vpRect.x.get() + x;
    int dstY = vpRect.y.get() + y;

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
 *   Private
 */

void Sprite::addToEngineCycles()
{
  if (addedToEngineCycles) {
    return;
  }
  Lists::Instance().addUpdateEntry(this);
  Lists::Instance().addRenderEntry(this);
  addedToEngineCycles = true;
}

void Sprite::removeFromEngineCycles()
{
  if (!addedToEngineCycles) {
    return;
  }
  Lists::Instance().removeUpdateEntry(this);
  Lists::Instance().removeRenderEntry(this);
  addedToEngineCycles = false;
}

void Sprite::setFlashStart(Color *color, int time)
{
  if (color) {
    flashColor = sf::Glsl::Vec4(
      color->red / 255.f,
      color->green / 255.f,
      color->blue / 255.f,
      color->alpha / 255.f
    );
    flashColorIsNil = false;
  } else {
    flashColorIsNil = true;
  }

  flashTicks = time;
  flashDuration = time;
}

}  // namespace Eng