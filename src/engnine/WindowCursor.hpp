#pragma once

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "aeon/toolkit/ElementBounds.h"
#include "engnine/Bitmap.h"
#include "engnine/EngineRenderEntity.hpp"
#include "engnine/Lists.hpp"
#include "engnine/Shaders.h"

namespace Eng {

using ae::ElementBounds;
using sf::RectangleShape;
using sf::RenderTexture;
using SfSprite = sf::Sprite;

class WindowCursor : public EngineRenderEntity {
 public:

  WindowCursor(sf::View& view, const Viewport* viewport) :
      view(view),
      viewport(viewport),
      shaders(Shaders::Instance()),
      cursorColor(sf::Color::White)
  {
    z = 0;
    opacity = 255;
    visible = true;
    isDisposed = false;
    isDirty = false;
    isReady = false;
    animationPosition = 0;
  }

  /*
    ⇩⇩⇩ Render List Methods ⇩⇩⇩
  */

  sf::Texture backTexture;

  void onRender(sf::RenderTexture& renderTexture) override
  {
    renderTexture.setView(view);
    renderTexture.draw(cursorSprite, sf::BlendAdd);
    renderTexture.display();
    renderTexture.setView(renderTexture.getDefaultView());
  }

  bool shouldRender() const override
  {
    return !isDisposed && visible && isReady;
  }

  int getZIndex() const override
  {
    return z;
  }

  /*
    ⇩⇩⇩ Self Methods ⇩⇩⇩
  */

  void update()
  {
    if (isDirty) {
      refresh();
    }
  }

  void updateAnimation()
  {
    if (!shouldRender()) {
      return;
    }

    static const uint8_t animationAlphaValues[] = {
      /* Fade out */
      0xFF, 0xF7, 0xEF, 0xE7, 0xDF, 0xD7, 0xCF, 0xC7,
      0xBF, 0xB7, 0xAF, 0xA7, 0x9F, 0x97, 0x8F, 0x87,
      /* Fade in */
      0x7F, 0x87, 0x8F, 0x97, 0x9F, 0xA7, 0xAF, 0xB7,
      0xBF, 0xC7, 0xCF, 0xD7, 0xDF, 0xE7, 0xEF, 0xF7
    };

    static const int animationValuesSize = sizeof(animationAlphaValues);

    animationPosition = (animationPosition + 1) % animationValuesSize;
    opacity = animationAlphaValues[animationPosition];
    cursorColor.a = opacity;
    cursorSprite.setColor(cursorColor);
  }

  void setBounds(const ElementBounds& value)
  {
    if (bounds == value) return;
    bounds = value;
    isDirty = true;
  }

  void setWindowSkin(const Bitmap* skin)
  {
    if (windowSkin == skin) return;
    windowSkin = skin;
    isDirty = true;
  }

  void setZIndex(int z_)
  {
    if (z == z_) return;
    z = z_;
    Lists::Instance().markZOrderDirty();
  }

  void setVisible(bool value)
  {
    visible = value;
  }

  void dispose()
  {
    isDisposed = true;
    removeToRenderList();
  }

  bool disposed()
  {
    return isDisposed;
  }

  /*
    ⇩⇩⇩ Private ⇩⇩⇩
  */

 private:
  sf::View& view;
  const Viewport* viewport;
  const Bitmap* windowSkin;
  int z;
  uint8_t opacity;
  uint8_t animationPosition;
  bool visible;
  bool isDisposed;
  bool isDirty;
  bool isReady;

  ElementBounds bounds;
  RenderTexture skinTexture;
  SfSprite skinSprite;
  RenderTexture cursorRenderTexture;
  RectangleShape cursorShape;
  SfSprite cursorSprite;
  sf::Color cursorColor;

  Shaders& shaders;

  void refresh()
  {
    isDirty = false;
    if (isDisposed || bounds.isEmpty() || !windowSkin) {
      isReady = false;
      return;
    }

    static const sf::Vector2f textSize = sf::Vector2f(32.0f, 32.0f);

    skinSprite.setTexture(windowSkin->renderTexture.getTexture());
    skinSprite.setTextureRect(sf::IntRect(128, 64, 32, 32));
    skinSprite.setPosition(0, 0);

    skinTexture.create(textSize.x, textSize.y);
    skinTexture.clear(sf::Color::Transparent);
    skinTexture.draw(skinSprite, sf::BlendAdd);
    skinTexture.display();

    shaders.windowCursorBg->setUniform("skinTexture", skinTexture.getTexture());
    shaders.windowCursorBg->setUniform("textureSize", textSize);
    shaders.windowCursorBg->setUniform("outputSize", bounds.size());

    cursorShape.setSize(bounds.size());
    cursorShape.setPosition(0, 0);

    cursorRenderTexture.create(bounds.size().x, bounds.size().y);
    cursorRenderTexture.clear(sf::Color::Transparent);
    cursorRenderTexture.draw(cursorShape, shaders.windowCursorBg.get());
    cursorRenderTexture.display();

    cursorSprite.setTexture(cursorRenderTexture.getTexture());
    cursorSprite.setPosition(bounds.position());

    isReady = true;
  }
};

}  // namespace Eng