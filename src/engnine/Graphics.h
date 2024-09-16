#pragma once

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "AppDefs.h"
#include "engnine/GraphicsRenderer.h"
namespace Eng {

using app::CStr;
using app::UInt;

class Graphics {
 public:
  /*
    ⇩⇩⇩ Static ⇩⇩⇩
  */

  static void Init(const char* title, sf::Vector2i& dimensions, sf::RenderWindow& window);
  static Graphics& GetInstance();
  static void Destroy();

  /*
    ⇩⇩⇩ Instance ⇩⇩⇩
  */

  unsigned long frame_count;

  sf::RenderTexture& getRenderTexture();

  UInt getFrameRate();

  void setFrameRate(UInt v);

  void update();

  void freeze();  // TODO: Implement this method

  void transition(int duration = 10, CStr fileName = nullptr, int vague = 40);

  // TODO: Implement this method
  void frame_reset();

  void toggleFullScreen();

  void adjustScreen();

 private:
  /*
    ⇩⇩⇩ Private ⇩⇩⇩
  */
  sf::Vector2i& dimensions;
  const char* title;

  bool isFullScreen;
  sf::RenderWindow& window;
  sf::View gameView;
  sf::RenderTexture rdt;
  sf::Sprite renderSprite;
  GraphicsRenderer renderer;
  sf::Image transitionImage;
  sf::Texture transitionTexture;

  UInt frame_rate;
  UInt timestamp;

  Graphics(const char* title, sf::Vector2i& dimensions, sf::RenderWindow& window);

  Graphics(const Graphics&);
  Graphics& operator=(const Graphics&);

  void setup();

  void frameUpdate();

  void doFadeTransition(int duration);

  void doImageTransition(int duration, CStr fileName, float vague);
};

}  // namespace Eng