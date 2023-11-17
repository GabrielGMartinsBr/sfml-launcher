#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <stdexcept>
#include <vector>

#include "base/Log.hpp"
#include "engnine/Sprite.hpp"
#include "engnine/Viewport.hpp"
#include "ruby.h"

namespace Eng {

class Engine {
  bool initialized = false;
  bool running = false;

  std::vector<VALUE> sprites;

  sf::RenderWindow* window = nullptr;
  sf::View view;

  Engine() :
      view(sf::FloatRect(0, 0, 640, 480))
  {
  }

  Engine(const Engine&);
  Engine& operator=(const Engine&);

 public:
  static Engine& getInstance()
  {
    static Engine instance;
    return instance;
  }

  void addSprite(VALUE sprite)
  {
    Log::out() << "Adding sprite";
    sprites.push_back(sprite);
    Log::out() << "Sprites number: " << sprites.size();
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

  std::vector<VALUE>* getSprites()
  {
    return &sprites;
  }

  void updateInput()
  {
    pollEvents();
  }

  void updateGraphics()
  {
    window->clear();

    renderSprites();

    window->display();
  }


 private:
  void renderSprites()
  {
    Eng::Sprite* spr = nullptr;

    for (VALUE i : sprites) {
      spr = (Eng::Sprite*)DATA_PTR(i);
      if (spr && spr->shouldRender()) {
        spr->applyChanges();
        window->draw(spr->sprite);
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