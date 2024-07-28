#pragma once

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "engnine/Autotiles.h"
#include "engnine/Drawable.hpp"
#include "engnine/EngineBase.hpp"
#include "engnine/Table.hpp"
#include "engnine/Viewport.hpp"

namespace Eng {

class Tilemap : public EngineBase, Drawable {
 public:

  Tilemap(Viewport* _viewport = nullptr);
  Tilemap(VALUE rbObj, Viewport* _viewport = nullptr);

  void bindRubyProps();

  /*
    Drawable
  */

  inline int getZPosition() const override;

  inline bool shouldRender() const override;

  void update() override;

  void draw(sf::RenderTexture& renderTexture) override;

  /*
    Properties
  */

  Bitmap* getter_tileset();
  void setter_tileset(Bitmap* value);

  Autotiles* getter_autotiles();

  Table* getter_map_data();
  void setter_map_data(Table* value);

  Table* getter_flash_data();
  void setter_flash_data(Table* value);

  Table* getter_priorities();
  void setter_priorities(Table* value);

  bool getter_visible();
  void setter_visible(bool value);

  int getter_ox();
  void setter_ox(int value);

  int getter_oy();
  void setter_oy(int value);

  /*
    Methods
  */

  void method_dispose();
  bool method_disposed();

  Viewport* method_viewport();

  void method_update();

 private:
  Viewport* viewport;
  Bitmap* tileset;
  Autotiles* autotiles;
  Table* map_data;
  Table* flash_data;
  Table* priorities;
  bool visible;
  int ox;
  int oy;

  bool isDisposed;
  bool isEligible;
  bool ready;
  bool dirty;
  sf::Sprite spr;
  sf::RenderTexture rTexture;

  int cols;
  int rows;
  int w;
  int h;

  const sf::Texture* autoTileTextures[7];

  void updateIsEligible();

  void handleTile(int x, int y, int z, sf::Sprite& tile);

  void handleAutoTile(int x, int y, int z);
};

}  // namespace Eng