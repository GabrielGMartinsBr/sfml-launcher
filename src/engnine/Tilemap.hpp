#pragma once

#include "engnine/Autotiles.hpp"
#include "engnine/Table.hpp"
#include "engnine/Viewport.hpp"
#include "ruby.h"

namespace Eng {

class Tilemap {
 public:
  VALUE ptr;

  Tilemap(Viewport* _viewport = nullptr)
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
  }

  /*
    Properties
  */

  // tileset

  Bitmap* getter_tileset()
  {
    return tileset;
  }

  void setter_tileset(Bitmap* value)
  {
    tileset = value;
  }

  // autotiles

  Autotiles* getter_autotiles()
  {
    return autotiles;
  }

  // map_data

  Table* getter_map_data()
  {
    return map_data;
  }

  void setter_map_data(Table* value)
  {
    map_data = value;
  }

  // flash_data

  Table* getter_flash_data()
  {
    return flash_data;
  }

  void setter_flash_data(Table* value)
  {
    flash_data = value;
  }

  // priorities

  Table* getter_priorities()
  {
    return priorities;
  }

  void setter_priorities(Table* value)
  {
    priorities = value;
  }

  // visible

  bool getter_visible()
  {
    return visible;
  }

  void setter_visible(bool value)
  {
    visible = value;
  }

  // ox

  int getter_ox()
  {
    return ox;
  }

  void setter_ox(int value)
  {
    ox = value;
  }

  // oy

  int getter_oy()
  {
    return oy;
  }

  void setter_oy(int value)
  {
    oy = value;
  }

  /*
    Methods
  */

  // dispose

  void method_dispose()
  {
    isDisposed = true;
  }

  // disposed?

  bool method_disposed()
  {
    return isDisposed;
  }

  // viewport

  Viewport* method_viewport()
  {
    return viewport;
  }

  // update

  void method_update() { }

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
};

}  // namespace Eng