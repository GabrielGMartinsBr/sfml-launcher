#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <thread>

#include "engnine/Engine.hpp"
#include "integrator/Integrator.hpp"
#include "launcher/LauncherTimer.hpp"

class Launcher {
  int width = 640;
  int height = 480;
  const char* title = "ORM S-Launcher";

  sf::VideoMode mode;
  sf::RenderWindow window;

  LauncherTimer timer;

  Integrator integrator;

  Eng::Engine& engine = Eng::Engine::getInstance();

 public:

  Launcher() :
      mode(width, height),
      window(mode, title) { }

  void run()
  {
    std::thread rbThread(integrator);

    while (window.isOpen()) {
      timer.update();

      if (timer.shouldUpdate()) {
        listenEvents();
        update();
        render();
        timer.markUpdate();
        // timer.printFps();
      } else {
        timer.sleep();
      }
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

  void update()
  {
  }

  void render()
  {
    window.clear();

    engine.render(window);

    window.display();
  }

  void handleClose()
  {
    window.close();
  }
};