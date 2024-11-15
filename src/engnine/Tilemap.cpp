#include "Tilemap.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cstddef>
#include <memory>
#include <stdexcept>

#include "engnine/AutotilePositions.h"
#include "engnine/Autotiles.h"
#include "engnine/Bitmap.h"
#include "engnine/Engine.h"
#include "engnine/EngineBase.hpp"
#include "engnine/Lists.hpp"
#include "engnine/Table.hpp"
#include "engnine/TilemapLayer.h"
#include "engnine/Viewport.hpp"
#include "integrator/It_Autotiles.hpp"
#include "integrator/It_Table.hpp"

namespace Eng {

using std::make_unique;

constexpr int T_SIZE = 32;
constexpr int T_COLS = 8;

Tilemap::Tilemap(Viewport* _viewport) :
    Tilemap(Qnil, _viewport) { }

Tilemap::Tilemap(VALUE rbObj, Viewport* viewport) :
    EngineBase(rbObj),
    dimensions(Engine::getInstance().getDimensions()),
    srcRect(0, 0, dimensions.x, dimensions.y),
    viewport(viewport),
    layersTable(0)
{
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
  created = false;
  shouldBuildSprites = true;
  shouldUpdateSprRect = true;
  dirty = false;
  addedToEngineCycles = false;
  layersN = 0;
  frameCount = 0;
  currentFrame = 0;
  layersIsSetup = false;

  if (rbObj != Qnil) {
    bindRubyProps();
  }

  addToEngineCycles();
}

Tilemap::~Tilemap()
{
  removeFromEngineCycles();
  disposeLayers();
}

void Tilemap::onUpdate()
{
  if (!dirty || tileset == nullptr || map_data == nullptr) {
    return;
  }

  if (!layersIsSetup) {
    setupLayers();
    if (!layersIsSetup) {
      std::runtime_error("Layers setup failed.");
    }
  }

  if (shouldBuildSprites) {
    buildSprites();
    shouldBuildSprites = false;
  }

  if (shouldUpdateSprRect) {
    srcRect.left = ox;
    srcRect.top = oy;
    int _oy = oy / 32;

    size_t len = layersTable.size();
    for (int i = 0; i < len; i++) {
      if (layersTable[i]) {
        layersTable[i]->update(_oy);
        layersTable[i]->updateSrcRect(srcRect);
      }
    }
    shouldUpdateSprRect = false;
  }

  dirty = false;
  ready = true;
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

  rb_iv_set(rbObj, "@autotiles", autotiles->rbObj);
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
  if (tileset == value) {
    return;
  }

  if (value->rbObj == Qnil) {
    value->rbObj = It::Bitmap::createRubyObject(value);
  }

  tileset = value;
  rb_iv_set(rbObj, "@tileset", tileset->rbObj);

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
  if (map_data == value) {
    return;
  }

  if (value->rbObj == Qnil) {
    value->rbObj = It::Table::createRubyObject(value);
  }

  map_data = value;
  rb_iv_set(rbObj, "@map_data", map_data->rbObj);

  updateIsEligible();
}

// flash_data

Table* Tilemap::getter_flash_data()
{
  return flash_data;
}

void Tilemap::setter_flash_data(Table* value)
{
  if (flash_data == value) {
    return;
  }

  if (value->rbObj == Qnil) {
    value->rbObj = It::Table::createRubyObject(value);
  }

  flash_data = value;
  rb_iv_set(rbObj, "@flash_data", flash_data->rbObj);
}

// priorities

Table* Tilemap::getter_priorities()
{
  return priorities;
}

void Tilemap::setter_priorities(Table* value)
{
  if (priorities == value) {
    return;
  }

  if (value->rbObj == Qnil) {
    value->rbObj = It::Table::createRubyObject(value);
  }

  priorities = value;
  rb_iv_set(rbObj, "@priorities", priorities->rbObj);
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
  dirty = true;
  shouldUpdateSprRect = true;
}

// oy

int Tilemap::getter_oy()
{
  return oy;
}

void Tilemap::setter_oy(int value)
{
  oy = value;
  dirty = true;
  shouldUpdateSprRect = true;
}

/*
  Methods
*/

// dispose

void Tilemap::method_dispose()
{
  isDisposed = true;
  removeFromEngineCycles();
  disposeLayers();
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

void Tilemap::method_update()
{
  frameCount++;
  currentFrame = (frameCount / 20) % 4;
}

/*
  Private
*/

void Tilemap::addToEngineCycles()
{
  if (addedToEngineCycles) {
    return;
  }
  Lists::Instance().addUpdateEntry(this);
  addedToEngineCycles = true;
}

void Tilemap::removeFromEngineCycles()
{
  if (!addedToEngineCycles) {
    return;
  }
  Lists::Instance().removeUpdateEntry(this);
  addedToEngineCycles = false;
}

void Tilemap::setupLayers()
{
  if (map_data == nullptr) {
    return;
  }
  int rows = map_data->getYSize();
  layersN = rows + 5;

  layersTable.resize(layersN, 1);

  layersIsSetup = true;
}

void Tilemap::checkLayer(int id, int y, int priority, int oy)
{
  if (layersTable.isEmpty(id, 0)) {
    int cols = map_data->getXSize();
    int rows = map_data->getYSize();
    int w = cols * 32;
    int h = rows * 32;
    UPtr<TilemapLayer> layer = make_unique<TilemapLayer>(viewport, w, h, y, priority, oy / 32, &currentFrame);
    layersTable.setValue(std::move(layer), id, 0);
  }
}

void Tilemap::updateIsEligible()
{
  isEligible = map_data != nullptr && tileset != nullptr;
  if (isEligible) {
    dirty = true;
  }
}

void Tilemap::buildSprites()
{
  const sf::Texture& tileTexture = tileset->getTexture();
  tileSprite.setTexture(tileTexture);

  Bitmap* bp;
  for (int i = 0; i < 7; i++) {
    bp = autotiles->getter(i);
    if (bp == nullptr) {
      autotileFrameN[i] = 0;
      continue;
    } else {
      autotileSpr[i].setTexture(bp->getTexture());
      autotileFrameN[i] = bp->getter_width() / 96;
    }
  }

  int cols = map_data->getXSize();
  int rows = map_data->getYSize();

  int w = cols * 32;
  int h = rows * 32;

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      handleTile(x, y, 0);
      handleTile(x, y, 1);
      handleTile(x, y, 2);
    }
  }

  size_t len = layersTable.size();
  for (int i = 0; i < len; i++) {
    if (layersTable[i]) {
      layersTable[i]->rendTex.display();
      layersTable[i]->frameRn[0].display();
      layersTable[i]->frameRn[1].display();
      layersTable[i]->frameRn[2].display();
      layersTable[i]->frameRn[3].display();
    }
  }

  int n = 0;
  for (int i = 0; i < len; i++) {
    if (layersTable[i]) {
      n++;
    }
  }
}

void Tilemap::handleTile(int x, int y, int z)
{
  int id = map_data->getValue(x, y, z);
  if (id < 48) {
    return;
  }

  if (id < 48 * 8) {
    handleAutoTile(id, x, y, z);
    return;
  }

  int priority = 0;
  int layerId = 0;
  if (priorities != nullptr) {
    priority = priorities->getValue(id);
    if (priority > 0) {
      layerId = y + priority;
    }
  }
  checkLayer(layerId, y, priority, oy);

  id -= 48 * 8;

  int tile_x = id % T_COLS * T_SIZE;
  int tile_y = id / T_COLS * T_SIZE;

  tileSprite.setTextureRect(sf::IntRect(tile_x, tile_y, T_SIZE, T_SIZE));
  tileSprite.setPosition(x * T_SIZE, y * T_SIZE);

  layersTable.getValue(layerId, 0).rendTex.draw(tileSprite);
}

void Tilemap::handleAutoTile(int id, int x, int y, int z)
{
  int autoTileId = id / 48 - 1;
  int autoTilePatter = id % 48;

  int priority = 0;
  int layerId = 0;
  if (priorities != nullptr) {
    priority = priorities->getValue(id);
    if (priority > 0) {
      layerId = y + priority;
    }
  }

  checkLayer(layerId, y, priority, oy);

  sf::Sprite& tile = autotileSpr[autoTileId];

  id %= 48;
  const int* tiles = autotilesPositions[id >> 3][id & 7];

  int _x = x * T_SIZE;
  int _y = y * T_SIZE;

  int tile_position, tx, ty;

  sf::IntRect tileSrcRect(0, 0, 16, 16);

  for (int i = 0; i < 4; i++) {
    tile_position = tiles[i] - 1;
    tx = (tile_position % 6) * 16;
    ty = (tile_position / 6) * 16;
    tileSrcRect.left = tx;
    tileSrcRect.top = ty;
    tile.setTextureRect(tileSrcRect);
    tile.setPosition(
      _x + i % 2 * 16,
      std::floor(_y + i / 2 * 16)
    );

    int frameN = autotileFrameN[autoTileId];

    if (frameN == 1) {
      layersTable.getValue(layerId, 0).rendTex.draw(tile);
    } else {
      for (int f = 0; f < 4; f++) {
        tileSrcRect.left = tx + 96 * f;
        tile.setTextureRect(tileSrcRect);
        layersTable.getValue(layerId, 0).frameRn[f].draw(tile);
      }
    }
  }
}
void Tilemap::disposeLayers()
{
  size_t len = layersTable.size();
  for (int i = 0; i < len; i++) {
    if (layersTable[i]) {
      layersTable[i]->dispose();
    }
  }
}

}  // namespace Eng