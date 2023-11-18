#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <memory>
#include <stdexcept>
#include <vector>

#include "base/Log.hpp"
#include "base/Sugars.hpp"
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
    initialized = true;
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
    viewports.push_back(vp);
  }

  /*
    Sprites
  */

  std::vector<VALUE>* getSprites()
  {
    return &sprites;
  }

  void addSprite(VALUE sprite)
  {
    Log::out() << "Adding sprite";
    sprites.push_back(sprite);
    Log::out() << "Sprites number: " << sprites.size();
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

    renderViewports();

    window->display();
  }


 private:
  bool initialized = false;
  bool running = false;

  RGSS::Viewport defaultViewport;
  Vector<SharedPtr<Eng::Viewport>> viewports;
  Vector<VALUE> sprites;

  sf::RenderWindow* window = nullptr;

  Engine() :
      defaultViewport(0, 0, 640, 480)
  {
  }

  Engine(const Engine&);
  Engine& operator=(const Engine&);

  void renderViewports()
  {
    updateSprites();

    defaultViewport.renderIn(*window);
    defaultViewport.display();

    for (SharedPtr<Eng::Viewport> vp : viewports) {
      if (vp) {
        vp->getRgssViewport().renderIn(*window);
        vp->getRgssViewport().display();
      }
    }
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