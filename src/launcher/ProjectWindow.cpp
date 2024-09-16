#include "launcher/ProjectWindow.h"

ProjectWindow::ProjectWindow(const app::String& title, const sf::Vector2i& dimensions) :
    title(title),
    dimensions(dimensions),
    mode(dimensions.x, dimensions.y),
    window(mode, title, sf::Style::Titlebar | sf::Style::Close)
{
}

int ProjectWindow::width() const
{
  return dimensions.x;
}

int ProjectWindow::height() const
{
  return dimensions.y;
}

void ProjectWindow::centralize()
{
  sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
  position.x = (desktop.width - dimensions.x) / 2;
  position.y = (desktop.height - dimensions.y) / 2;

  window.setPosition(position);
}

void ProjectWindow::setFullScreen()
{
  window.create(sf::VideoMode::getDesktopMode(), title, sf::Style::Fullscreen);
}

void ProjectWindow::setWindowMode()
{
  window.create(sf::VideoMode(dimensions.x, dimensions.y), title, sf::Style::Titlebar | sf::Style::Close);
  centralize();
}

void ProjectWindow::close()
{
  window.close();
}