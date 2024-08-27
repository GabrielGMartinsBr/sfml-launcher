#include "Engine.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <stdexcept>

#include "AppDefs.h"
#include "engnine/BlenShaders.hpp"
#include "engnine/EngineRenderer.h"
#include "engnine/FileUtils.hpp"
#include "engnine/Input.hpp"
#include "ruby.h"

namespace Eng {

Engine& Engine::getInstance()
{
  static Engine instance;
  return instance;
}

/*
  Getters
*/

bool Engine::isRunning()
{
  return running;
}

const sf::Vector2i& Engine::getDimensions() const
{
  return dimensions;
}

const app::String& Engine::getProjectPath() const
{
  return projectPath;
}

const app::String& Engine::getScriptsPath() const
{
  return scriptsPath;
}

/*
  Methods
*/

void Engine::init(sf::RenderWindow& _window, app::CStr _projectPath)
{
  if (initialized) {
    throw std::runtime_error("Engine can be initialized only once.");
  }
  running = true;
  window = &_window;
  projectPath = _projectPath;

  resolvePaths();

  // Log::out() << scriptsPath;

  blendShaders.loadShaders();

  auto size = window->getSize();

  window->display();

  renderer = new EngineRenderer(size.x, size.y);
  initialized = true;
}

void Engine::update()
{
  pollEvents();
}

void Engine::cleanup()
{
  delete renderer;
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

void Engine::stop()
{
  handleCloseEvent();
}

/*
  Private
*/

Engine::Engine() :
    input(Input::getInstance()),
    dimensions(640, 480)
{
  initialized = false;
  running = false;
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

void Engine::resolvePaths()
{
  app::FilePath _scriptsPath = FileUtils::combine(projectPath, "/Data/Scripts.rxdata");
  scriptsPath = _scriptsPath.c_str();
}

}  // namespace Eng