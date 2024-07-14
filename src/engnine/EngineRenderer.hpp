#pragma once

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <stdexcept>

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
    createDefaultViewport();
    loadShaders();
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
    // renderTexture.clear(sf::Color::Transparent);
    renderTexture.clear();
    // clearViewports();
    renderSprites();
    // renderViewports();
    renderBuffer(target);
  }

  sf::RenderTexture& getRenderTexture()
  {
    return renderTexture;
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

  sf::Shader vpNormalShader;
  sf::Shader sprNormalShader;
  sf::Shader sprInvertShader;

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

  void loadShaders()
  {
    if (
      !sprNormalShader.loadFromFile("../shaders/sprite_normal-blend.frag", sf::Shader::Fragment)
      || !sprInvertShader.loadFromFile("../shaders/sprite_invert-blend.frag", sf::Shader::Fragment)
      || !vpNormalShader.loadFromFile("../shaders/viewport_normal-blend.frag", sf::Shader::Fragment)
    ) {
      throw std::runtime_error("Failed to load shader.");
    }
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

    float opacity = spr->getter_opacity() / 255.f;

    sf::RenderStates state;

    state.blendMode = sf::BlendNone;

    if (spr->getBlendType() == 2) {
      sprInvertShader.setUniform("opacity", opacity);
      state.shader = &sprInvertShader;
      state.blendMode = sf::BlendMultiply;
    } else {
      state.blendMode = sf::BlendAlpha;
      // sprNormalShader.setUniform("opacity", opacity);
      // state.shader = &sprNormalShader;
    }

    // vp->getRgssViewport().draw(
    //   sfSprite,
    //   state
    // );
    // vp->getRgssViewport().display();

    renderTexture.draw(
      sfSprite,
      state
    );
    renderTexture.display();
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
};

}  // namespace Eng