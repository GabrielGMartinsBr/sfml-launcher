#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "AppDefs.h"

struct ProjectWindow {
  const app::String& title;
  const sf::Vector2i& dimensions;

  sf::Vector2i position;
  sf::VideoMode mode;
  sf::RenderWindow window;

  ProjectWindow(const app::String& title, const sf::Vector2i& dimensions);

  int width() const;
  int height() const;

  void centralize();

  void setFullScreen();

  void setWindowMode();

  void close();
};