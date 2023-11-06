#pragma once

#include <SFML/System/Vector3.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

struct GameInput {
  bool up;
  bool down;
  bool left;
  bool right;
  sf::Vector2i cursor;
  bool button;
  bool mouseOnScreen;

  void changeKeyState(sf::Event::KeyEvent evt, bool b);

  void updateCursor(uint x, uint y);
};
