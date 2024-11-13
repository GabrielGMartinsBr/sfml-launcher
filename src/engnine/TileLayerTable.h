#pragma once

#include "AppDefs.h"
#include "engnine/TilemapLayer.h"

namespace Eng {

using app::UPtr;
using app::Vector;

struct TileLayerTable {
  TileLayerTable(int x, int y = 1);

  const UPtr<TilemapLayer>& operator[](size_t index) const;

  bool isEmpty(int x, int y = 0);

  TilemapLayer& getValue(int x, int y = 0);

  void setValue(UPtr<TilemapLayer> layer, int x, int y = 0);

  void dispose(int x, int y = 0);

  int xSize() const;
  int ySize() const;
  int size() const;

  void resize(int x);
  void resize(int x, int y);

 private:
  int xSizeVal;
  int ySizeVal;
  int sizeVal;

  Vector<UPtr<TilemapLayer>> values;

  void fill();

  inline const int calcIndex(int x, int y = 0) const;
};

}  // namespace Eng