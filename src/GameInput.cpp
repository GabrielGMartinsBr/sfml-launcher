#include "GameInput.h"

#include <SFML/Window/Keyboard.hpp>

void GameInput::changeKeyState(sf::Event::KeyEvent evt, bool b)
{
  switch (evt.code) {
    case sf::Keyboard::Left:
      left = b;
      break;
    case sf::Keyboard::Up:
      up = b;
      break;
    case sf::Keyboard::Right:
      right = b;
      break;
    case sf::Keyboard::Down:
      down = b;
      break;
    default:
      break;
  }
}

void GameInput::updateCursor(uint x, uint y)
{
  if (mouseOnScreen) {
    cursor.x = x;
    cursor.y = y;
  }
}
