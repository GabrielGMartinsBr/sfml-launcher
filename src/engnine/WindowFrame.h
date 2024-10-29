#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "engnine/Bitmap.h"
#include "engnine/IOnRender.h"
#include "engnine/Viewport.hpp"

namespace Eng {

struct WindowFrame : IOnRender {
  sf::Sprite backSprite;
  sf::Sprite borderSprite;
  sf::Texture texture;
  sf::RenderTexture rendTex;
  int width;
  int height;

  int visible;

  WindowFrame(Viewport* viewport);

  ~WindowFrame();

  void onRender(sf::RenderTexture& renderTexture) override;

  bool shouldRender() const override;

  int getZIndex() const override;

  void dispose();

  sf::Sprite& getSprite();

  void setZ(int v);

  void setOpacity(int v);
  int getOpacity();

  void update(Bitmap* windowSkin);

 private:
  Viewport* viewport;
  int z;
  int opacity;

  bool isDisposed;
  bool addedToEngineCycles;

  sf::Color color;

  void addToEngineCycles();

  void removeFromEngineCycles();

  void updateBackgroundSprite(Bitmap* windowSkin);

  void updateBorder(Bitmap* windowSkin);
};

}  // namespace Eng