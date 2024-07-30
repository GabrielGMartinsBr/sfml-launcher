#pragma once

#include "engnine/Engine.h"

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
#include "engnine/Input.hpp"
#include "engnine/internal/Texts.hpp"
#include "ruby.h"

namespace Eng {

Engine& Engine::getInstance()
{
  static Engine instance;
  return instance;
}

BlenShaders blendShaders;

void Engine::init(sf::RenderWindow& _window)
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

EngineRenderer* Engine::getRenderer()
{
  return renderer;
}

void Engine::cleanup()
{
  delete renderer;
}

bool Engine::isRunning()
{
  return running;
}

void Engine::addViewport(SharedPtr<Eng::Viewport> vp)
{
  renderer->addViewport(vp);
}

void Engine::addDrawable(Eng::Drawable* drawable)
{
  renderer->addDrawable(drawable);
}

void Engine::removeDrawable(Eng::Drawable* drawable)
{
  renderer->removeDrawable(drawable);
}

void Engine::markZOrderDirty()
{
  renderer->markZOrderDirty();
}

void Engine::updateInput()
{
  pollEvents();
}

void Engine::updateGraphics()
{
  window->clear();

  renderer->render(window);

  window->display();
}


Engine::Engine() :
    input(Input::getInstance())
{
}

void Engine::pollEvents()
{
  sf::Event event;
  while (window->pollEvent(event)) {
    switch (event.type) {
      case sf::Event::Closed:
        handleCloseEvent();
        break;
      case sf::Event::KeyPressed:
        input.handleKeyPressed(event.key);
        break;
      case sf::Event::KeyReleased:
        input.handleKeyRelease(event.key);
        break;
      default:
        break;
    }
  }
}

void Engine::handleCloseEvent()
{
  ruby_stop(0);
  running = false;
}

}  // namespace Eng