#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

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

  void init(sf::RenderWindow& _window);

  EngineRenderer* getRenderer();

  void cleanup();

  bool isRunning();

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
  bool initialized = false;
  bool running = false;
  Input& input;

  sf::RenderWindow* window;
  EngineRenderer* renderer;

  Engine();

  Engine(const Engine&);
  Engine& operator=(const Engine&);

  void pollEvents();

  void handleCloseEvent();
};

}  // namespace Eng