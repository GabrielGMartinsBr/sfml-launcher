#pragma once

#include <SFML/Graphics/Texture.hpp>
namespace Eng {

struct Drawable {
  virtual int getZPosition() = 0;

  virtual void update() = 0;

  virtual void draw(sf::RenderTexture& renderTexture) = 0;
};

}  // namespace Eng