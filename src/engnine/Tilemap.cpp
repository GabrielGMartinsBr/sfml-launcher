#include "engnine/Tilemap.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cmath>

#include "engnine/AutotilePositions.h"
#include "engnine/Autotiles.h"
#include "engnine/Bitmap.h"
#include "engnine/Engine.hpp"
#include "engnine/EngineBase.hpp"
#include "engnine/Table.hpp"
#include "engnine/Viewport.hpp"
#include "integrator/It_Autotiles.hpp"

namespace Eng {

constexpr int T_SIZE = 32;
constexpr int T_COLS = 8;

Tilemap::Tilemap(Viewport* _viewport) :
    Tilemap(Qnil, _viewport) { }

Tilemap::Tilemap(VALUE rbObj, Viewport* _viewport) :
    EngineBase(rbObj)
{
  viewport = _viewport;
  tileset = nullptr;
  autotiles = new Autotiles();
  map_data = nullptr;
  flash_data = nullptr;
  priorities = nullptr;
  visible = true;
  ox = 0;
  oy = 0;
  isDisposed = false;

  isEligible = false;
  ready = false;
  dirty = false;

  if (rbObj != Qnil) {
    bindRubyProps();
  }

  Eng::Engine::getInstance().addDrawable(this);
}

// Engine Drawable

inline int Tilemap::getZPosition() const
{
  return 0;
}

inline bool Tilemap::shouldRender() const
{
  return isEligible;
}

void Tilemap::update()
{
  if (!dirty || tileset == nullptr || map_data == nullptr) {
    return;
  }

  const sf::Texture& tileTexture = tileset->renderTexture.getTexture();

  cols = map_data->getXSize();
  rows = map_data->getYSize();

  w = cols * 32;
  h = rows * 32;

  rTexture.create(w, h);

  // sf::Image img;
  // img.create(w, h, sf::Color::Transparent);

  sf::Sprite tile;
  tile.setTexture(tileTexture);

  Bitmap* autoTileBitmap;
  for (int i = 0; i < 7; i++) {
    autoTileBitmap = autotiles->getter(i);
    if (autoTileBitmap == nullptr) {
      autoTileTextures[i] = nullptr;
    } else {
      autoTileTextures[i] = &autoTileBitmap->renderTexture.getTexture();
    }
  }

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      handleTile(x, y, 0, tile);
      handleTile(x, y, 1, tile);
      handleTile(x, y, 2, tile);
    }
  }

  rTexture.display();
  spr.setTexture(rTexture.getTexture());

  dirty = false;
  ready = true;
}

void Tilemap::draw(sf::RenderTexture& rd)
{
  if (!ready) return;
  rd.draw(spr);
  rd.display();
}

/*
  Bind ruby props
*/

void Tilemap::bindRubyProps()
{
  if (rbObj == Qnil) {
    std::runtime_error("Tilemap doesn't have rbObj defined.");
  }

  if (autotiles->rbObj == Qnil) {
    autotiles->rbObj = It::Autotiles::createRubyObject(autotiles);
  }

  rb_iv_set(rbObj, "autotiles", autotiles->rbObj);
}

/*
  Properties
*/

// tileset

Bitmap* Tilemap::getter_tileset()
{
  return tileset;
}

void Tilemap::setter_tileset(Bitmap* value)
{
  tileset = value;
  updateIsEligible();
}

// autotiles

Autotiles* Tilemap::getter_autotiles()
{
  return autotiles;
}

// map_data

Table* Tilemap::getter_map_data()
{
  return map_data;
}

void Tilemap::setter_map_data(Table* value)
{
  map_data = value;
  updateIsEligible();
}

// flash_data

Table* Tilemap::getter_flash_data()
{
  return flash_data;
}

void Tilemap::setter_flash_data(Table* value)
{
  flash_data = value;
}

// priorities

Table* Tilemap::getter_priorities()
{
  return priorities;
}

void Tilemap::setter_priorities(Table* value)
{
  priorities = value;
}

// visible

bool Tilemap::getter_visible()
{
  return visible;
}

void Tilemap::setter_visible(bool value)
{
  visible = value;
}

// ox

int Tilemap::getter_ox()
{
  return ox;
}

void Tilemap::setter_ox(int value)
{
  ox = value;
}

// oy

int Tilemap::getter_oy()
{
  return oy;
}

void Tilemap::setter_oy(int value)
{
  oy = value;
}

/*
  Methods
*/

// dispose

void Tilemap::method_dispose()
{
  isDisposed = true;
}

// disposed?

bool Tilemap::method_disposed()
{
  return isDisposed;
}

// viewport

Viewport* Tilemap::method_viewport()
{
  return viewport;
}

// update

void Tilemap::method_update() { }

void Tilemap::updateIsEligible()
{
  isEligible = map_data != nullptr && tileset != nullptr;
  if (isEligible) {
    dirty = true;
  }
}

void Tilemap::handleTile(int x, int y, int z, sf::Sprite& tile)
{
  int id = map_data->getValue(x, y, z);
  if (id < 48) {
    return;
  }

  if (id < 48 * 8) {
    handleAutoTile(id, x, y);
    return;
  }

  id -= 48 * 8;

  int tile_x = id % T_COLS * T_SIZE;
  int tile_y = id / T_COLS * T_SIZE;

  tile.setTextureRect(sf::IntRect(tile_x, tile_y, T_SIZE, T_SIZE));
  tile.setPosition(x * T_SIZE, y * T_SIZE);
  rTexture.draw(tile);
}

void Tilemap::handleAutoTile(int id, int x, int y)
{
  int autoTileId = id / 48 - 1;
  int autoTilePatter = id % 48;

  const sf::Texture* aTexture = autoTileTextures[autoTileId];

  if (aTexture == nullptr) {
    return;
  }

  sf::Sprite aTile;
  aTile.setTexture(*aTexture);

  id %= 48;
  const int* tiles = autotilesPositions[id >> 3][id & 7];

  int _x = x * T_SIZE;
  int _y = y * T_SIZE;

  int tile_position, tx, ty;
  for (int i = 0; i <= 4; i++) {
    tile_position = tiles[i] - 1;
    tx = (tile_position % 6) * 16;
    ty = (tile_position / 6) * 16;
    aTile.setTextureRect(
      sf::IntRect(tx, ty, 16, 16)
    );
    aTile.setPosition(
      _x + i % 2 * 16,
      std::floor(_y + i / 2 * 16)
    );
    rTexture.draw(aTile);
  }
}

}  // namespace Eng