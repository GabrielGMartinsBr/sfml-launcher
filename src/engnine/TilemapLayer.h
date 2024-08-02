#pragma once

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include "engnine/OnRender.h"

namespace Eng {

class TilemapLayer : OnRender {
 public:
  sf::Sprite sprite;
  sf::RenderTexture rendTex;

  TilemapLayer(int width, int height, int y, int priority, int layersN, int oy);

  ~TilemapLayer();

  // Engine Cycles

  int getZIndex() const override;

  bool shouldRender() const override;

  void onRender(sf::RenderTexture& renderTexture) override;

  void update(int oy);

  // Self

  void dispose();

 private:
  int y;
  int priority;
  int layersN;
  int z;
  int nextZ;
  bool isDisposed;
  bool addedToEngineCycles;

  void create(int width, int height);

  void calcZ();

  void addToEngineCycles();
  void removeFromEngineCycles();
};

}  // namespace Eng