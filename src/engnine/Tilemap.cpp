#pragma once

#include "engnine/Tilemap.h"

#include "engnine/Autotiles.h"
#include "engnine/Engine.hpp"
#include "engnine/EngineBase.hpp"
#include "engnine/Table.hpp"
#include "engnine/Viewport.hpp"
#include "integrator/It_Autotiles.hpp"

namespace Eng {

Tilemap::Tilemap(Viewport* _viewport) :
    Tilemap(Qnil, _viewport) { }

Tilemap::Tilemap(VALUE rbObj, Viewport* _viewport) :
    EngineBase(rbObj)
{
  viewport = _viewport;
  tileset = nullptr;
  autotiles = new Autotiles();
  map_data = nullptr;
  map_data = nullptr;
  flash_data = nullptr;
  priorities = nullptr;
  visible = true;
  ox = 0;
  oy = 0;
  isDisposed = false;

  if (rbObj != Qnil) {
    bindRubyProps();
  }

  Eng::Engine::getInstance().addDrawable(this);
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

// Drawable

inline int Tilemap::getZPosition() const
{
  return 0;
}

inline bool Tilemap::shouldRender() const
{
  return false;
}

void Tilemap::update()
{
}

void Tilemap::draw(sf::RenderTexture& renderTexture)
{
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

}  // namespace Eng