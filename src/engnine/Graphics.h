#pragma once

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "AppDefs.h"
#include "engnine/GraphicsRenderer.h"
#include "launcher/ProjectWindow.h"

namespace Eng {

using app::CStr;
using app::String;
using app::UInt;

class Graphics {
 public:
  /*
    ⇩⇩⇩ Static ⇩⇩⇩
  */

  static void Init(ProjectWindow& projectWindow);
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

  int getWidth();
  int getHeight();

 private:
  /*
    ⇩⇩⇩ Private ⇩⇩⇩
  */
  ProjectWindow& projectWindow;

  bool isFullScreen;
  sf::View gameView;
  sf::RenderTexture rdt;
  sf::Sprite renderSprite;
  GraphicsRenderer renderer;
  sf::Image transitionImage;
  sf::Texture transitionTexture;

  UInt frame_rate;
  UInt timestamp;

  Graphics(ProjectWindow& projectWindow);

  Graphics(const Graphics&);
  Graphics& operator=(const Graphics&);

  void setup();

  void frameUpdate();

  void doFadeTransition(int duration);

  void doImageTransition(int duration, CStr fileName, float vague);
};

}  // namespace Eng