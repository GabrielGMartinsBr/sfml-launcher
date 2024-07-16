#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <stdexcept>

#include "base/Sugars.hpp"
#include "engnine/BlenShaders.hpp"
#include "engnine/Drawable.hpp"
#include "engnine/EngineRenderer.hpp"
#include "engnine/internal/Texts.hpp"
#include "ruby.h"

namespace Eng {

class Engine {
 public:
  static Engine& getInstance()
  {
    static Engine instance;
    return instance;
  }

  BlenShaders blendShaders;

  void init(sf::RenderWindow& _window)
  {
    if (initialized) {
      throw std::runtime_error("Engine can be initialized only once.");
    }
    running = true;
    window = &_window;

    blendShaders.loadShaders();

    Texts::loadFonts();
    auto size = window->getSize();

    window->display();

    renderer = new EngineRenderer(size.x, size.y);
    initialized = true;
  }

  EngineRenderer* getRenderer()
  {
    return renderer;
  }

  void cleanup()
  {
    delete renderer;
  }

  bool isRunning()
  {
    return running;
  }

  void addViewport(SharedPtr<Eng::Viewport> vp)
  {
    renderer->addViewport(vp);
  }

  void addDrawable(Eng::Drawable* drawable)
  {
    renderer->addDrawable(drawable);
  }

  void markZOrderDirty()
  {
    renderer->markZOrderDirty();
  }

  void updateInput()
  {
    pollEvents();
  }

  void updateGraphics()
  {
    window->clear();

    renderer->render(window);

    window->display();
  }


 private:
  bool initialized = false;
  bool running = false;

  sf::RenderWindow* window = nullptr;
  EngineRenderer* renderer = nullptr;

  Engine()
  {
  }

  Engine(const Engine&);
  Engine& operator=(const Engine&);

  void pollEvents()
  {
    sf::Event event;
    while (window->pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          handleCloseEvent();
          break;
        default:
          break;
      }
    }
  }

  void handleCloseEvent()
  {
    ruby_stop(0);
    running = false;
  }
};

}  // namespace Eng