#pragma once

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "aeon/toolkit/ElementBounds.h"
#include "core/Vec2f.hpp"
#include "engnine/Bitmap.h"
#include "engnine/EngineRenderEntity.hpp"
#include "engnine/Lists.hpp"
#include "engnine/Shaders.h"
#include "engnine/WindowFrame.h"

namespace Eng {

using ae::ElementBounds;
using sf::RectangleShape;
using sf::RenderTexture;
using SfSprite = sf::Sprite;
using Core::Vec2f;

class WindowCursor : public EngineRenderEntity {
 public:
  const WindowFrame& frame;

  WindowCursor(sf::View& view, const WindowFrame& frame, const Viewport* viewport) :
      view(view),
      frame(frame),
      viewport(viewport),
      shaders(Shaders::Instance()),
      cursorColor(sf::Color::White)
  {
    z = 0;
    opacity = 255;
    isDisposed = false;
    isDirty = false;
    isReady = false;
  }

  /*
    ⇩⇩⇩ Render List Methods ⇩⇩⇩
  */

  sf::Texture backTexture;

  void onRender(sf::RenderTexture& renderTexture) override
  {
    // Set the current view for rendering
    renderTexture.setView(view);

    // Obtain frame's sprite and texture details
    const sf::Sprite& frameSprite = frame.backSprite;
    backTexture = *frameSprite.getTexture();

    // const sf::Vector2f& size = bounds.size();

    // frame.getZIndex()

    // Define texture size, rectangle position, and size in texture coordinates
    sf::Vector2f backTextureSize(frame.width, frame.height);
    // sf::Vector2f backTextureSize(backTexture.getSize());
    sf::Vector2f rectPos(frameSprite.getTextureRect().left, frameSprite.getTextureRect().top);
    sf::Vector2f rectSize(frameSprite.getTextureRect().width, frameSprite.getTextureRect().height);

    // Calculate the scaling ratio between the cursor sprite and frame texture
    sf::Vector2u rSize = backTexture.getSize();
    sf::Vector2u sSize = cursorSprite.getTexture()->getSize();
    // sf::Vector2f sSize = bounds.size();
    // sf::Vector2f size(static_cast<float>(sSize.x) / rSize.x, static_cast<float>(sSize.y) / rSize.y);

    // Calculate the offset for the center of the cursor within the frame
    sf::Vector2f offset(
      ((backTextureSize.x - sSize.x) / 2) / backTextureSize.x,
      ((backTextureSize.y - sSize.y) / 2) / backTextureSize.y
    );

    Vec2f textureSize(cursorSprite.getTexture()->getSize());

    Vec2f size(sSize);
    size /= backTextureSize;
    // size /= rSize;

    // Scale the rectangle coordinates to a normalized range (0.0 to 1.0)
    ElementBounds frameCoord(rectPos, rectSize);
    frameCoord /= textureSize.sfVector2f();

    sf::Vector2f texCoordEnd(frameCoord.endX(), frameCoord.endY());

    // Set uniforms for the shader
    shaders.backTextureRectBlend->setUniform("backTexture", backTexture);
    shaders.backTextureRectBlend->setUniform("backTextureSize", backTextureSize);
    shaders.backTextureRectBlend->setUniform("backOpacity", frameSprite.getColor().a / 255.f);
    shaders.backTextureRectBlend->setUniform("texCoordStart", frameCoord.position());
    shaders.backTextureRectBlend->setUniform("texCoordSize", frameCoord.size());
    shaders.backTextureRectBlend->setUniform("texCoordEnd", texCoordEnd);
    shaders.backTextureRectBlend->setUniform("offset", offset);
    shaders.backTextureRectBlend->setUniform("size", size.sfVector2f());
    shaders.backTextureRectBlend->setUniform("spriteSize", bounds.size());

    shaders.backTextureRectBlend->setUniform("textureSize", textureSize.sfVector2f());
    shaders.backTextureRectBlend->setUniform("opacity", opacity / 255.f);

    // Render cursor sprite with the shader
    sf::RenderStates state;
    state.shader = shaders.backTextureRectBlend.get();
    // state.blendMode = sf::BlendAlpha;

    // renderTexture.draw(cursorSprite, state);
    renderTexture.draw(cursorSprite, sf::BlendAdd);

    // Log::out() << "texCoordEnd";
    // Log::out() << backTextureSize.x;
    // Log::out() << backTextureSize.y;
    // Log::out() << "opacity: " << frameSprite.getColor().a / 255.f;

    // Display renderTexture and reset to default view
    renderTexture.display();
    renderTexture.setView(renderTexture.getDefaultView());
  }

  bool shouldRender() const override
  {
    return isReady && !isDisposed;
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
  bool isDisposed;
  bool isDirty;
  bool isReady;

  ElementBounds bounds;
  RenderTexture skinTexture;
  SfSprite skinSprite;
  RenderTexture cursorRenderTexture;
  RectangleShape cursorShape;
  SfSprite cursorSprite;

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