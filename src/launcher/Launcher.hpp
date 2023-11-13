#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

#include "engnine/Engine.hpp"
#include "integrator/Integrator.hpp"

class Launcher {
  int width = 640;
  int height = 480;
  const char* title = "ORM S-Launcher";

  sf::VideoMode mode;
  sf::RenderWindow window;

  Integrator integrator;

  Eng::Engine& engine = Eng::Engine::getInstance();

 public:

  Launcher() :
      mode(width, height),
      window(mode, title) { }

  void run()
  {
    engine.init(window);
    integrator.init();

    window.close();
  }
};