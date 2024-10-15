
#include "engnine/Graphics.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include "AppDefs.h"
#include "NumberUtils.hpp"
#include "aeon/window/AeonWindowManager.h"
#include "engnine/Audio.h"
#include "engnine/Engine.h"
#include "engnine/FileUtils.hpp"
#include "engnine/Timer.hpp"
#include "launcher/ProjectWindow.h"

namespace Eng {

/*
  ⇩⇩⇩ Static ⇩⇩⇩
*/
static Graphics* instance = nullptr;

void Graphics::Init(ProjectWindow& projectWindow)
{
  assert(!instance);
  instance = new Graphics(projectWindow);
}

Graphics& Graphics::GetInstance()
{
  assert(instance);
  return *instance;
}

void Graphics::Destroy()
{
  assert(instance);
  delete instance;
  instance = nullptr;
}

/*
  ⇩⇩⇩ Instance ⇩⇩⇩
*/

Graphics::Graphics(ProjectWindow& projectWindow) :
    projectWindow(projectWindow),
    gameView(sf::FloatRect(0, 0, projectWindow.width(), projectWindow.height())),
    renderer(projectWindow, rdt)
{
  isFullScreen = false;
  frame_rate = 40;
  frame_count = 0;
  timestamp = 0;
  setup();
}

sf::RenderTexture& Graphics::getRenderTexture()
{
  return rdt;
}

unsigned int Graphics::getFrameRate()
{
  return frame_rate;
}

void Graphics::setFrameRate(unsigned int v)
{
  if (v < 10) {
    v = 10;
  } else if (v > 120) {
    v = 120;
  }
  frame_rate = v;
  Timer::getInstance().setFrameRate(frame_rate);
}

void Graphics::update()
{
  ae::AeonWindowManager::Instance().updateEntries();
  renderer.render();
  frameUpdate();
}

void Graphics::freeze()
{
  renderer.freeze();
}

void Graphics::transition(int duration, CStr fileName, int _vague)
{
  if (fileName == nullptr) {
    doFadeTransition(duration);
  } else {
    float vague = Num::clamp(_vague, 1, 256) / 256.0f;
    doImageTransition(duration, fileName, vague);
  }
}

// TODO: Implement this method
void Graphics::frame_reset()
{
  Log::out() << "Graphics frame_reset was called, but it is not implemented yet.";
}

void Graphics::toggleFullScreen()
{
  if (isFullScreen) {
    projectWindow.setWindowMode();
    isFullScreen = false;
  } else {
    projectWindow.setFullScreen();
    isFullScreen = true;
  }
  adjustScreen();
}

void Graphics::adjustScreen()
{
  sf::Vector2u gameResolution(projectWindow.dimensions);
  sf::Vector2u screenResolution = projectWindow.window.getSize();

  float gameAspectRatio = static_cast<float>(gameResolution.x) / gameResolution.y;
  float screenAspectRatio = static_cast<float>(screenResolution.x) / screenResolution.y;

  float scaleFactor;
  sf::FloatRect viewport;

  if (screenAspectRatio > gameAspectRatio) {
    scaleFactor = static_cast<float>(screenResolution.y) / gameResolution.y;
    viewport.width = (gameAspectRatio / screenAspectRatio);
    viewport.height = 1.f;
    viewport.left = (1.f - viewport.width) / 2.f;  // Center horizontally
    viewport.top = 0.f;
  } else {
    scaleFactor = static_cast<float>(screenResolution.x) / gameResolution.x;
    viewport.width = 1.f;
    viewport.height = (screenAspectRatio / gameAspectRatio);
    viewport.left = 0.f;
    viewport.top = (1.f - viewport.height) / 2.f;
  }

  gameView.setViewport(viewport);
  gameView.setSize(gameResolution.x, gameResolution.y);
  projectWindow.window.setView(gameView);
}

/*
  ⇩⇩⇩ Extended Methods ⇩⇩⇩
*/

int Graphics::getWidth()
{
  return projectWindow.dimensions.x;
}

int Graphics::getHeight()
{
  return projectWindow.dimensions.y;
}

/*
  ⇩⇩⇩ Private ⇩⇩⇩
*/

void Graphics::setup()
{
  adjustScreen();

  sf::ContextSettings settings;
  settings.antialiasingLevel = 0;
  rdt.create(projectWindow.width(), projectWindow.height(), settings);
  rdt.clear(sf::Color::Transparent);
}

void Graphics::frameUpdate()
{
  Engine::getInstance().updateInput();
  Timer::getInstance().controlFrameRate();
  Audio::Instance().update(timestamp);

  frame_count++;
  timestamp = frame_count / frame_rate;
}

void Graphics::doFadeTransition(int duration)
{
  int end = frame_count + duration;

  while (frame_count <= end) {
    float progress = 1 - static_cast<float>(end - frame_count) / duration;
    renderer.renderFadeTransitionState(progress);
    frameUpdate();
  }

  renderer.transitionEnd();
}

void Graphics::doImageTransition(int duration, CStr fileName, float vague)
{
  int end = frame_count + duration;

  app::String filename = FileUtils::resolveRtpImagePath(fileName);
  bool loaded = transitionImage.loadFromFile(filename);
  if (!loaded) {
    throw std::runtime_error("Could not load image.");
  }
  transitionTexture.loadFromImage(transitionImage);

  while (frame_count <= end) {
    float progress = 1 - static_cast<float>(end - frame_count) / duration;
    renderer.renderImageTransitionState(progress, transitionTexture, vague);
    frameUpdate();
  }

  renderer.transitionEnd();
}

}  // namespace Eng