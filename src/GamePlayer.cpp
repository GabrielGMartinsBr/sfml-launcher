#include "GamePlayer.h"

#include <SFML/System/Vector2.hpp>

extern GameInput game_input;

static const unsigned int w = 24;
static const unsigned int h = 32;

GamePlayer::GamePlayer(unsigned int init_x, unsigned int init_y) :
    charsTexture(loadCharsetTexture()),
    spriteRect(0, 0, w, h),
    sprite(charsTexture, spriteRect)
{
  x = init_x;
  y = init_y;
  sprite.setPosition(x, y);
}

void GamePlayer::update()
{
  if (game_input.right) {
    next_x = x + 1;
  } else if (game_input.left) {
    next_x = x - 1;
  }

  if (game_input.down) {
    next_y = y + 1;
  } else if (game_input.up) {
    next_y = y - 1;
  }

  if (next_x > 0 && next_x + w < 900) {
    x = next_x;
  }
  if (next_y > 0 && next_y + h < 600) {
    y = next_y;
  }

  sprite.setPosition(x, y);
}

void GamePlayer::draw(sf::RenderWindow &win)
{
  win.draw(sprite);
}