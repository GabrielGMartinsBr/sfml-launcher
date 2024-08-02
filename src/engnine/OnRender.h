#pragma once

#include <SFML/Graphics/RenderTexture.hpp>

namespace Eng {

struct OnRender {
  virtual void onRender(sf::RenderTexture& renderTexture) = 0;

  virtual bool shouldRender() const = 0;

  virtual int getZIndex() const = 0;
};

}  // namespace Eng