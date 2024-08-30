
#include "engnine/Graphics.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "NumberUtils.hpp"
#include "engnine/Audio.h"
#include "engnine/Engine.h"
#include "engnine/FileUtils.hpp"
#include "engnine/Timer.hpp"

namespace Eng {

/*
  ⇩⇩⇩ Static ⇩⇩⇩
*/
static Graphics* instance = nullptr;

void Graphics::Init(UInt width, UInt height, sf::RenderWindow& window)
{
  assert(!instance);
  instance = new Graphics(width, height, window);
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

Graphics::Graphics(UInt width, UInt height, sf::RenderWindow& window) :
    width(width),
    height(height),
    window(window),
    renderer(window, rdt)
{
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

/*
  ⇩⇩⇩ Private ⇩⇩⇩
*/

void Graphics::setup()
{
  sf::ContextSettings settings;
  settings.antialiasingLevel = 0;
  rdt.create(width, height, settings);
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