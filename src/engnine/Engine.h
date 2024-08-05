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
#include "engnine/OnRender.h"
#include "engnine/OnUpdate.h"

namespace Eng {

class Engine {
 public:
  static Engine& getInstance();

  BlenShaders blendShaders;

  // Getters

  EngineRenderer* getRenderer();

  bool isRunning();

  const sf::Vector2i& getDimensions() const;

  const app::String& getProjectPath() const;

  const app::String& getScriptsPath() const;

  // Methods

  void init(sf::RenderWindow& _window, app::CStr projectPath);

  void cleanup();

  void addViewport(SharedPtr<Eng::Viewport> vp);

  // Update List

  void addToUpdateList(Eng::OnUpdate* instance);

  void removeFromUpdateList(Eng::OnUpdate* instance);

  // Render List

  void addToRenderList(Eng::OnRender* instance);

  void removeFromRenderList(Eng::OnRender* instance);

  void markZOrderDirty();

  // Engine update methods

  void updateInput();

  void updateGraphics();

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