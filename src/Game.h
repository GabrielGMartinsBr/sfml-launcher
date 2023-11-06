#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

#include "GameDialog.h"
#include "GamePlayer.h"

class Game {
  GamePlayer player;
  GameDialog dialog;

 public:
  Game();

  void update();
  
  void draw(sf::RenderWindow &win);
};
