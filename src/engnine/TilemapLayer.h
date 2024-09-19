#pragma once

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include "engnine/IViewportChild.h"
#include "engnine/Viewport.hpp"

namespace Eng {

class TilemapLayer : IViewportChild {
 public:
  sf::Sprite sprite;
  sf::RenderTexture rendTex;
  bool visible;

  TilemapLayer(Viewport* viewport, int width, int height, int y, int priority, int oy);

  ~TilemapLayer();

  // Engine Cycles

  inline Viewport* getViewport() const override;

  int getZIndex() const override;

  bool shouldRender() const override;

  void onRender(sf::RenderTexture& renderTexture) override;

  void update(int oy);

  // Self

  void dispose();

 private:
  int y;
  int oy;
  int priority;
  int z;
  int nextZ;
  bool isDisposed;
  bool addedToEngineCycles;
  Viewport* viewport;

  void create(int width, int height);

  void calcZ();

  void addToEngineCycles();
  void removeFromEngineCycles();
};

}  // namespace Eng