#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <thread>

#include "graphics/Graphics.hpp"
#include "integrator/Integrator.hpp"

class Launcher {
  int width = 600;
  int height = 500;
  const char *title = "SFML Launcher";

  sf::VideoMode mode;
  sf::RenderWindow window;

  Integrator integrator;
  Graphics gr;

 public:

  Launcher() :
      mode(width, height),
      window(mode, title)
  {
  }

  void run()
  {
    std::thread rbThread(integrator);

    while (window.isOpen()) {
      listenEvents();
      update();
      render();
    }

    rbThread.join();
  }

 private:

  void listenEvents()
  {
    sf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          handleClose();
          break;
        default:
          // std::cout << event.type << '\n';
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