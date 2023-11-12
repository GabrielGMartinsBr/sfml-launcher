#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#include "base/Log.hpp"
#include "graphics/Graphics.hpp"
#include "integrator/Integrator.hpp"

class Launcher {
  int width = 600;
  int height = 500;
  const char *title = "SFML Launcher";

  sf::VideoMode mode;
  sf::RenderWindow window;

  float elapsed;
  float fps;
  std::chrono::high_resolution_clock::time_point currentTime;
  std::chrono::high_resolution_clock::time_point previousTime;
  float frameTime = 1.0f / 60.0f * 1e6;

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
      currentTime = std::chrono::high_resolution_clock::now();
      elapsed = (float)std::chrono::duration_cast<std::chrono::microseconds>(currentTime - previousTime).count();

      if (elapsed >= frameTime) {
        listenEvents();
        update();
        render();

        fps = (float)1e6 / elapsed;
        Log::out() << "FPS: " << std::floor(fps);

        previousTime = currentTime;
      } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
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

    gr.render(window);

    window.display();
  }

  void handleClose()
  {
    window.close();
  }
};