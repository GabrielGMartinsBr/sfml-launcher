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

  static void Init(UInt width, UInt height, sf::RenderWindow& window);
  static Graphics& GetInstance();
  static void Destroy();

  /*
    ⇩⇩⇩ Instance ⇩⇩⇩
  */

  unsigned long frame_count;

  sf::RenderTexture& getRenderTexture();

  unsigned int getFrameRate();

  void setFrameRate(unsigned int v);

  void update();

  void freeze();  // TODO: Implement this method

  void transition(int duration = 10, CStr fileName = nullptr, int vague = 40);

  // TODO: Implement this method
  void frame_reset();

 private:
  /*
    ⇩⇩⇩ Private ⇩⇩⇩
  */
  UInt width;
  UInt height;
  sf::RenderWindow& window;
  sf::RenderTexture rdt;
  sf::Sprite renderSprite;
  GraphicsRenderer renderer;

  unsigned int frame_rate;

  Graphics(UInt width, UInt height, sf::RenderWindow& window);

  Graphics(const Graphics&);
  Graphics& operator=(const Graphics&);

  void setup();

  void updateGraphics();
};

}  // namespace Eng