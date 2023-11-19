#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

#include "base/Sugars.hpp"
#include "engnine/Sprite.hpp"
#include "engnine/Viewport.hpp"

namespace Eng {

struct EngineRenderer {
  EngineRenderer(
    unsigned int _width, unsigned int _height
  ) :
      width(_width),
      height(_height)
  {
    createBuffer();
    defaultVp = MakeSharedPtr<Eng::Viewport>(0, 0, width, height);
    viewports.push_back(defaultVp);
  }

  void addViewport(SharedPtr<Eng::Viewport> vp)
  {
    viewports.push_back(vp);
  }

  void addSprite(Eng::Sprite* spr)
  {
    sprites.push_back(spr);
  }

  void render(sf::RenderTarget* target)
  {
    clearViewports();
    renderSprites();
    renderViewports();
    renderBuffer(target);
  }

 private:
  unsigned int width;
  unsigned int height;

  sf::RenderWindow* window;
  Vector<SharedPtr<Eng::Viewport>> viewports;
  Vector<Eng::Sprite*> sprites;
  SharedPtr<Eng::Viewport> defaultVp;

  sf::Sprite bufferSprite;
  sf::RenderTexture bufferTexture;

  void createBuffer()
  {
    bufferTexture.create(width, height);
    bufferTexture.clear(sf::Color::Transparent);
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

  void renderSprites()
  {
    for (Eng::Sprite* spr : sprites) {
      if (!spr) {
        continue;
      }
      spr->renderInViewport(
        defaultVp->getRgssViewport()
      );
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
      bufferTexture
    );
    vp.getRgssViewport().display();
  }

  void renderBuffer(sf::RenderTarget* target)
  {
    bufferTexture.display();
    bufferSprite.setTexture(bufferTexture.getTexture());
    target->draw(bufferSprite);
  }
};

}  // namespace Eng