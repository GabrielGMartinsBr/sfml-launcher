#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "graphics/Graphics.hpp"

class Launcher {
  int width = 600;
  int height = 500;
  const char *title = "SFML Launcher";

  sf::VideoMode mode;
  sf::RenderWindow window;

  Graphics gr;

 public:

  Launcher() :
      mode(width, height),
      window(mode, title)
  {
  }

  void run()
  {
    while (window.isOpen()) {
      pollEvent();
      update();
      render();
    }
  }

 private:

  void pollEvent()
  {
    sf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          handleClose();
          break;
        default:
          break;
      }
    }
  }

  void update() { }

  void render()
  {
    window.clear();

    gr.render(window);

    window.display();
  }

  void handleClose()
  {
    window.close();
  }
};