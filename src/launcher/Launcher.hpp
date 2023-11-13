#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

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
    engine.init(window);
    integrator.init();

    // while (engine.isRunning()) {
    //   timer.update();
    //   if (timer.shouldUpdate()) {
    //     update();
    //   } else {
    //     timer.sleep();
    //   }
    // }

    window.close();
  }

 private:
  void update()
  {
    engine.updateInput();
    // engine.updateGraphics();

    timer.markUpdate();
    timer.printFps();
  }

  void handleClose()
  {
    window.close();
  }
};