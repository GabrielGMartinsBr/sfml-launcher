
#include "engnine/Graphics.h"

#include <SFML/Graphics/Color.hpp>

#include "engnine/Engine.h"
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
    renderer(rdt)
{
  frame_rate = 40;
  frame_count = 0;
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
  updateGraphics();
  Engine::getInstance().updateInput();

  // Timer::getInstance().printFps();
  Timer::getInstance().controlFrameRate();

  frame_count++;
}

// TODO: Implement this method
void Graphics::freeze() { }

void Graphics::transition(int duration, CStr fileName, int vague)
{
  int end = frame_count + duration;
  while (frame_count <= end) {
    update();
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

void Graphics::updateGraphics()
{
  window.clear();

  renderer.render(window);

  window.display();
}

}  // namespace Eng