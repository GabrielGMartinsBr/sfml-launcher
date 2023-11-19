#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <memory>
#include <stdexcept>
#include <vector>

#include "base/Log.hpp"
#include "base/Sugars.hpp"
#include "engnine/EngineRenderer.hpp"
#include "engnine/RGSSViewport.hpp"
#include "engnine/Sprite.hpp"
#include "ruby.h"

namespace Eng {

class Engine {
 public:
  static Engine& getInstance()
  {
    static Engine instance;
    return instance;
  }

  void init(sf::RenderWindow& _window)
  {
    if (initialized) {
      throw std::runtime_error("Engine can be initialized only once.");
    }
    running = true;
    window = &_window;
    window->display();
    auto size = window->getSize();
    renderer = new EngineRenderer(size.x, size.y);
    initialized = true;
  }

  void cleanup()
  {
    delete renderer;
  }

  bool isRunning()
  {
    return running;
  }

  /*
    Viewports
  */

  RGSS::Viewport& getDefaultViewport()
  {
    return defaultViewport;
  }

  void addViewport(SharedPtr<Eng::Viewport> vp)
  {
    renderer->addViewport(vp);
  }

  /*
    Sprites
  */

  std::vector<VALUE>* getSprites()
  {
    return &sprites;
  }

  void addSprite(VALUE spriteValue)
  {
    Eng::Sprite* spr = (Eng::Sprite*)DATA_PTR(spriteValue);
    renderer->addSprite(spr);
  }

  /*
    Others
  */

  void updateInput()
  {
    pollEvents();
  }

  void updateGraphics()
  {
    window->clear();

    renderer->render(window);

    window->display();
  }


 private:
  bool initialized = false;
  bool running = false;

  RGSS::Viewport defaultViewport;
  Vector<SharedPtr<Eng::Viewport>> viewports;
  Vector<VALUE> sprites;

  sf::RenderWindow* window = nullptr;
  EngineRenderer* renderer = nullptr;

  Engine() :
      defaultViewport(0, 0, 640, 480)
  {
  }

  Engine(const Engine&);
  Engine& operator=(const Engine&);

  void renderViewports()
  {
    defaultViewport.clear();
    for (SharedPtr<Eng::Viewport> vp : viewports) {
      if (vp) {
        vp->getRgssViewport().clear();
      }
    }

    updateSprites();

    defaultViewport.renderIn(*window);
    defaultViewport.display();

    for (SharedPtr<Eng::Viewport> vp : viewports) {
      if (vp) {
        // defaultViewport.draw(
        //   vp->getRgssViewport().sprite
        // );
        vp->getRgssViewport().renderIn(*window);
        // vp->getRgssViewport().renderIn(defaultViewport);
        vp->getRgssViewport().display();
      }
    }

    // defaultViewport.renderIn(*window);
    // defaultViewport.display();
  }

  void updateSprites()
  {
    Eng::Sprite* spr = nullptr;
    for (VALUE i : sprites) {
      spr = (Eng::Sprite*)DATA_PTR(i);
      if (spr) {
        spr->renderInViewport(defaultViewport);
      }
    }
  }

  void pollEvents()
  {
    sf::Event event;
    while (window->pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          handleCloseEvent();
          break;
        default:
          break;
      }
    }
  }

  void handleCloseEvent()
  {
    ruby_stop(0);
    running = false;
  }
};

}  // namespace Eng