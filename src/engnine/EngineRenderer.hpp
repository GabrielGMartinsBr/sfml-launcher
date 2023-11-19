#pragma once

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
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
  sf::RenderTexture renderTexture;

  void createBuffer()
  {
    renderTexture.create(width, height);
    renderTexture.clear(sf::Color::Transparent);
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
      renderSprite(spr);
    }
  }

  void renderSprite(Eng::Sprite* spr)
  {
    if (!spr->shouldRender()) {
      return;
    }
    spr->applyChanges();

    sf::Sprite& sfSprite = spr->getSfSprite();
    Viewport* vp = spr->getViewport();

    if (!vp) {
      vp = defaultVp.get();
    }

    vp->getRgssViewport().draw(
      sfSprite
    );
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
      renderTexture
    );
    vp.getRgssViewport().display();
  }

  void renderBuffer(sf::RenderTarget* target)
  {
    renderTexture.display();
    bufferSprite.setTexture(renderTexture.getTexture());
    target->draw(bufferSprite);
  }
};

}  // namespace Eng