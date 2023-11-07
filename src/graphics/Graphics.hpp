#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

class Graphics {

 public:
  sf::Vector2f pos;
  sf::Vector2f size;
  sf::RectangleShape shape;

  Graphics() :
      pos(100, 100),
      size(200, 200),
      shape(size)
  {
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(pos);
  }

  void render(sf::RenderWindow &win)
  {
    win.draw(shape);
  }
};