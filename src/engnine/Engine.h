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
#include "engnine/Input.hpp"

namespace Eng {

using app::CStr;

class Engine {
 public:
  /*
    ⇩⇩⇩ Static ⇩⇩⇩
  */

  static void Init(sf::RenderWindow& window, CStr projectPath);
  static Engine& getInstance();
  static void Destroy();

  /*
    ⇩⇩⇩ Instance ⇩⇩⇩
  */

  BlenShaders blendShaders;

  // Getters

  bool isRunning();

  const sf::Vector2i& getDimensions() const;

  const app::String& getProjectPath() const;

  const app::String& getScriptsPath() const;

  // Methods

  void run();

  void update();

  void cleanup();

  // Engine update methods

  void updateInput();

  void stop();

 private:
  bool running;
  sf::Vector2i dimensions;
  app::String projectPath;
  app::String scriptsPath;

  Input& input;

  sf::RenderWindow& window;

  Engine(sf::RenderWindow& window, CStr projectPath);

  Engine(const Engine&);
  Engine& operator=(const Engine&);

  void pollEvents();

  void handleCloseEvent();

  void resolvePaths();
};

}  // namespace Eng