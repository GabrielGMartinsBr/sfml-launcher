#pragma once

#include "AppDefs.h"
#include "engnine/TilemapLayer.h"

namespace Eng {

using app::UPtr;
using app::Vector;

struct TileLayerTable {
  TileLayerTable(int x, int y = 1, int z = 1);

  bool isEmpty(int x, int y = 0, int z = 0);

  TilemapLayer& getValue(int x, int y = 0, int z = 0);

  void setValue(UPtr<TilemapLayer> layer, int x, int y = 0, int z = 0);

  void dispose(int x, int y = 0, int z = 0);

  int xSize() const;
  int ySize() const;
  int zSize() const;
  int size() const;

  void resize(int x);
  void resize(int x, int y);
  void resize(int x, int y, int z);


 private:
  int xSizeVal;
  int ySizeVal;
  int zSizeVal;
  int sizeVal;

  Vector<UPtr<TilemapLayer>> values;

  void fill();

  inline const int calcIndex(int x, int y = 0, int z = 0) const;
};

}  // namespace Eng