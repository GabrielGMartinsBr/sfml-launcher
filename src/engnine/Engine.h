#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "AppDefs.h"
#include "aeon/window/AeonWindowManager.h"
#include "engine/EngineClock.h"
#include "launcher/ProjectWindow.h"

namespace Eng {

using app::CStr;

class Engine {
 public:
  /*
    ⇩⇩⇩ Static ⇩⇩⇩
  */

  static void Init(ProjectWindow& projectWindow, CStr projectPath);
  static Engine& getInstance();
  static void Destroy();

  /*
    ⇩⇩⇩ Instance ⇩⇩⇩
  */

  // Getters

  EngineClock& clock;

  bool isRunning();

  const sf::Vector2i& getDimensions() const;

  const app::String& getProjectPath() const;

  const app::String& getScriptsPath() const;

  // Methods

  void run();

  void loopUpdate();

  void update();

  void cleanup();

  // Engine update methods

  void stop();

 private:
  ae::AeonWindowManager& aeonWinMng;

  bool running;
  ProjectWindow& projectWindow;
  app::String projectPath;
  app::String scriptsPath;

  Engine(ProjectWindow& projectWindow, CStr projectPath);

  Engine(const Engine&);
  Engine& operator=(const Engine&);

  void pollEvents();

  void handleCloseEvent();

  void resolvePaths();
};

}  // namespace Eng