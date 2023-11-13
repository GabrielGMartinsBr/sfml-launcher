#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <vector>

#include "base/Log.hpp"
#include "engnine/Sprite.hpp"
#include "ruby.h"

namespace Eng {

class Engine {
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

  std::vector<VALUE>* getSprites()
  {
    return &sprites;
  }

  void render(sf::RenderWindow& win)
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
};

}  // namespace Eng