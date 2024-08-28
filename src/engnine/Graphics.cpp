
#include "engnine/Graphics.h"

#include "engnine/Engine.h"
#include "engnine/Timer.hpp"

namespace Eng {

/*
  ⇩⇩⇩ Static ⇩⇩⇩
*/
static Graphics* instance = nullptr;

void Graphics::Init()
{
  assert(!instance);
  instance = new Graphics();
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

Graphics::Graphics()
{
  frame_rate = 40;
  frame_count = 0;
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
  Engine::getInstance().updateGraphics();
  Engine::getInstance().updateInput();

  // Timer::getInstance().printFps();
  Timer::getInstance().controlFrameRate();

  frame_count++;
}

// TODO: Implement this method
void Graphics::freeze() { }

// TODO: Implement this method
void Graphics::transition()
{
  Log::out() << "Graphics transition was called, but it is not implemented yet.";
}

// TODO: Implement this method
void Graphics::frame_reset()
{
  Log::out() << "Graphics frame_reset was called, but it is not implemented yet.";
}

}  // namespace Eng