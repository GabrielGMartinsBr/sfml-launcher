#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

#include "Game.h"
#include "GameFonts.h"
#include "GameInput.h"

static const unsigned int w = 900;
static const unsigned int h = 600;

static const unsigned int borderStroke = 6;
sf::Color borderColor = sf::Color::Cyan;

GameInput game_input;

int main(int, char **)
{
  sf::RenderWindow window(sf::VideoMode(w, h), "SFML works!");

  GameFonts::load();
  Game game;

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          break;

        case sf::Event::KeyPressed:
          game_input.changeKeyState(event.key, true);
          break;
        case sf::Event::KeyReleased:
          game_input.changeKeyState(event.key, false);
          break;

        case sf::Event::MouseLeft:
          game_input.mouseOnScreen = false;
          break;
        case sf::Event::MouseEntered:
          game_input.mouseOnScreen = true;
          break;
        case sf::Event::MouseMoved:
          game_input.updateCursor(event.mouseMove.x, event.mouseMove.y);
          break;
        case sf::Event::MouseButtonPressed:
          game_input.button = true;
          break;
        case sf::Event::MouseButtonReleased:
          game_input.button = false;
          break;

        default:
          // std::cout << event.type << '\n';
          break;
      }
    }

    game.update();

    window.clear();
    game.draw(window);

    window.display();
  }

  std::cout << "SFML Game works!" << std::endl;
}
