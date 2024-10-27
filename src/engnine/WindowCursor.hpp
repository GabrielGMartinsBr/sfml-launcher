#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "aeon/toolkit/ElementBounds.h"
#include "engnine/Bitmap.h"
#include "engnine/EngineRenderEntity.hpp"
#include "engnine/Lists.hpp"
#include "engnine/Shaders.h"

namespace Eng {

using ae::ElementBounds;
using sf::RectangleShape;
using SfSprite = sf::Sprite;

class WindowCursor : public EngineRenderEntity {
 public:

  WindowCursor(sf::View& view, const Viewport* viewport) :
      view(view),
      viewport(viewport),
      shaders(Shaders::Instance())
  {
    z = 0;
    isDisposed = false;
    isDirty = false;
    isReady = false;
  }

  /*
    ⇩⇩⇩ Render List Methods ⇩⇩⇩
  */

  void onRender(sf::RenderTexture& renderTexture) override
  {
    renderTexture.setView(view);
    renderTexture.draw(shape);
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
  SfSprite sprite;
  RectangleShape shape;

  Shaders& shaders;

  void refresh()
  {
    isDirty = false;
    if (isDisposed || bounds.isEmpty() || !windowSkin) {
      isReady = false;
      return;
    }

    shape.setSize(bounds.size());
    shape.setPosition(bounds.position());

    shape.setFillColor(sf::Color::Blue);

    isReady = true;
  }
};

}  // namespace Eng