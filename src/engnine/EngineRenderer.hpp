#pragma once

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include "base/Log.hpp"
#include "base/Sugars.hpp"
#include "engnine/Drawable.hpp"
#include "engnine/Viewport.hpp"

namespace Eng {

struct EngineRenderer {
  EngineRenderer(
    unsigned int _width, unsigned int _height
  ) :
      width(_width),
      height(_height),
      zDirty(false)
  {
    createBuffer();
    createDefaultViewport();
  }

  void addViewport(SharedPtr<Eng::Viewport> vp)
  {
    viewports.push_back(vp);
  }

  void addDrawable(Eng::Drawable* drawable)
  {
    drawables.push_back(drawable);
    zDirty = true;
  }

  void markZOrderDirty()
  {
    zDirty = true;
  }

  void render(sf::RenderTarget* target)
  {
    // renderTexture.clear(sf::Color::Transparent);
    renderTexture.clear();
    // clearViewports();
    renderDrawables();
    // renderViewports();
    renderBuffer(target);
  }

  sf::RenderTexture& getRenderTexture()
  {
    return renderTexture;
  }

  /**
   *
   *  Private starts here
   *
   */

 private:
  unsigned int width;
  unsigned int height;
  bool zDirty;

  sf::RenderWindow* window;
  Vector<SharedPtr<Eng::Viewport>> viewports;
  Vector<Eng::Drawable*> drawables;
  SharedPtr<Eng::Viewport> defaultVp;

  sf::Sprite bufferSprite;
  sf::RenderTexture renderTexture;

  void createBuffer()
  {
    sf::ContextSettings settings;
    // settings.antialiasingLevel = 0;
    renderTexture.create(width, height, settings);
    renderTexture.clear(sf::Color::Transparent);
  }

  void createDefaultViewport()
  {
    defaultVp = MakeSharedPtr<Eng::Viewport>(0, 0, width, height);
    viewports.push_back(defaultVp);
  }

  void clearViewports()
  {
    for (auto& vp : viewports) {
      if (!vp) {
        continue;
      }
      vp->getRgssViewport().clear();
    }
  }

  void renderDrawables()
  {
    if (zDirty) {
      sortZ();
    }
    for (Eng::Drawable* drawable : drawables) {
      drawable->update();
      drawable->draw(renderTexture);
    }
  }

  void renderViewports()
  {
    for (SharedPtr<Eng::Viewport>& vp : viewports) {
      if (!vp) {
        continue;
      }
      renderViewport(*vp);
    }
  }

  void renderViewport(Eng::Viewport& vp)
  {
    vp.getRgssViewport().renderIn(
      renderTexture, sf::BlendAlpha
    );
    vp.getRgssViewport().display();
  }

  void renderBuffer(sf::RenderTarget* target)
  {
    renderTexture.display();
    bufferSprite.setTexture(renderTexture.getTexture());
    target->draw(bufferSprite);
  }

  static bool compareZ(const Drawable* a, const Drawable* b)
  {
    return a->getZPosition() < b->getZPosition();
  }

  void sortZ()
  {
    Log::out() << "sort";
    std::sort(drawables.begin(), drawables.end(), compareZ);
    zDirty = false;
  }
};

}  // namespace Eng