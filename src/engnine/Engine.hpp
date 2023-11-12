#pragma once

#include <vector>

#include "base/Log.hpp"
#include "ruby.h"

namespace Eng {

class Engine {
  int c = 0;

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
};

}  // namespace Eng