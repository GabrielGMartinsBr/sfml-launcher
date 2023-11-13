#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <vector>

#include "base/Log.hpp"
#include "engnine/Sprite.hpp"
#include "ruby.h"

namespace Eng {

class Engine {
  bool running = false;
  std::vector<VALUE> sprites;

  Engine() { }

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

  void init()
  {
    running = true;
  }

  bool isRunning()
  {
    return running;
  }

  std::vector<VALUE>* getSprites()
  {
    return &sprites;
  }

  void updateInput(sf::RenderWindow& window)
  {
    pollEvents(window);
  }

  void updateGraphics(sf::RenderWindow& window)
  {
    window.clear();

    renderSprites(window);

    window.display();
  }


 private:
  void renderSprites(sf::RenderWindow& win)
  {
    Eng::Sprite* spr = nullptr;

    for (VALUE i : sprites) {
      spr = (Eng::Sprite*)DATA_PTR(i);
      if (spr && spr->shouldRender()) {
        spr->atualizar();
        win.draw(spr->sprite);
      }
    }
  }

  void pollEvents(sf::RenderWindow& win)
  {
    sf::Event event;
    while (win.pollEvent(event)) {
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
    running = false;
  }
};

}  // namespace Eng