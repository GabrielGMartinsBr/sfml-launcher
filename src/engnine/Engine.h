#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "AppDefs.h"
#include "engnine/BlenShaders.hpp"
#include "engnine/EngineRenderer.h"
#include "engnine/Input.hpp"

namespace Eng {

class Engine {
 public:
  static Engine& getInstance();

  BlenShaders blendShaders;

  // Getters

  bool isRunning();

  const sf::Vector2i& getDimensions() const;

  const app::String& getProjectPath() const;

  const app::String& getScriptsPath() const;

  // Methods

  void init(sf::RenderWindow& _window, app::CStr projectPath);

  void update();

  void cleanup();

  // Engine update methods

  void updateInput();

  void updateGraphics();

  void stop();

 private:
  bool initialized;
  bool running;
  sf::Vector2i dimensions;
  app::String projectPath;
  app::String scriptsPath;

  Input& input;

  sf::RenderWindow* window;
  EngineRenderer* renderer;

  Engine();

  Engine(const Engine&);
  Engine& operator=(const Engine&);

  void pollEvents();

  void handleCloseEvent();

  void resolvePaths();
};

}  // namespace Eng