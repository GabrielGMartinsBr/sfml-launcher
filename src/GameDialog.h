#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

class GameDialog {
  float border_width;
  float frame_height;

  sf::Vector2f position;
  sf::Vector2f size;

  sf::Vector2f client_size;
  sf::Vector2f client_position;

  sf::RectangleShape backShape;
  sf::RectangleShape clientShape;
  sf::Text title_text;

  sf::Vector2i& cursor;

  bool grabbed;
  sf::Vector2f grab_origin;

 public:
  GameDialog(
    const sf::String& title,
    float x, float y, float width, float height
  );

  void update();

  void draw(sf::RenderWindow& win);

 private:

  void clampPosition();
  void updatePosition();
};
