#include "Engine.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "AppDefs.h"
#include "engnine/FileUtils.hpp"
#include "engnine/Graphics.h"
#include "engnine/Input.hpp"
#include "ruby.h"

namespace Eng {

/*
  ⇩⇩⇩ Static ⇩⇩⇩
*/

static Engine* instance = nullptr;

void Engine::Init(sf::RenderWindow& window, CStr projectPath)
{
  assert(!instance);
  instance = new Engine(window, projectPath);
}

Engine& Engine::getInstance()
{
  assert(instance);
  return *instance;
}

void Engine::Destroy()
{
  assert(instance);
  delete instance;
  instance = nullptr;
}

/*
  ⇩⇩⇩ Instance ⇩⇩⇩
*/

Engine::Engine(sf::RenderWindow& window, CStr projectPath) :
    input(Input::getInstance()),
    dimensions(640, 480),
    window(window),
    projectPath(projectPath)
{
  running = false;
  resolvePaths();
}

void Engine::run()
{
  running = true;

  auto size = window.getSize();

  window.display();
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

void Engine::update()
{
  pollEvents();
}

void Engine::cleanup()
{
}

void Engine::updateInput()
{
  pollEvents();
}

void Engine::stop()
{
  handleCloseEvent();
}

/*
  Private
*/

void Engine::pollEvents()
{
  sf::Event event;
  while (window.pollEvent(event)) {
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