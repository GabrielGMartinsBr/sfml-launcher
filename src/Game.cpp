#include "Game.h"

Game::Game() :
    player(10, 10),
    dialog("Game Dialog", 50, 50, 160, 128)
{
}

void Game::update()
{
  player.update();
  dialog.update();
}

void Game::draw(sf::RenderWindow& win)
{
  player.draw(win);
  dialog.draw(win);
}