#include "Engine.h"

#include <ruby.h>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "AppDefs.h"
#include "aeon/window/AeonWindowManager.h"
#include "engnine/FileUtils.hpp"
#include "engnine/Graphics.h"
#include "engnine/Input.hpp"

namespace Eng {

/*
  ⇩⇩⇩ Static ⇩⇩⇩
*/

static Engine* instance = nullptr;

void Engine::Init(ProjectWindow& projectWindow, CStr projectPath)
{
  assert(!instance);
  instance = new Engine(projectWindow, projectPath);
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

Engine::Engine(ProjectWindow& projectWindow, CStr projectPath) :
    input(Input::getInstance()),
    aeonWinMng(ae::AeonWindowManager::Instance()),
    projectWindow(projectWindow),
    projectPath(projectPath)
{
  running = false;
  resolvePaths();
}

void Engine::run()
{
  running = true;

  projectWindow.window.display();
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
  return projectWindow.dimensions;
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
  while (projectWindow.window.pollEvent(event)) {
    switch (event.type) {
      case sf::Event::Closed:
        handleCloseEvent();
        break;
      case sf::Event::KeyPressed:
        if (event.key.alt && event.key.code == sf::Keyboard::Enter) {
          Graphics::GetInstance().toggleFullScreen();
          continue;
        }
        input.handleKeyPressed(event.key);
        aeonWinMng.handleKeyPressed(event.key);
        break;
      case sf::Event::KeyReleased:
        input.handleKeyRelease(event.key);
        break;
      case sf::Event::MouseMoved:
        aeonWinMng.handleMouseMoved(event.mouseMove);
        break;
      case sf::Event::MouseButtonPressed:
        aeonWinMng.handleMousePressed(event.mouseButton);
        break;
      case sf::Event::MouseButtonReleased:
        aeonWinMng.handleMouseReleased(event.mouseButton);
        break;
      case sf::Event::TextEntered:
        aeonWinMng.handleTextEntered(event.text);
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