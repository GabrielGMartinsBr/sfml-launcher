#include "engnine/TileLayerTable.h"

#include <memory>

namespace Eng {

/*
    ⇩⇩⇩ Public ⇩⇩⇩
*/

TileLayerTable::TileLayerTable(int x, int y, int z) :
    xSizeVal(x),
    ySizeVal(y),
    zSizeVal(z),
    sizeVal(x * y * z),
    values(sizeVal)
{
}

bool TileLayerTable::isEmpty(int x, int y, int z)
{
  return values[calcIndex(x, y, z)] == nullptr;
}

TilemapLayer& TileLayerTable::getValue(int x, int y, int z)
{
  return *values[calcIndex(x, y, z)];
}

void TileLayerTable::setValue(UPtr<TilemapLayer> layer, int x, int y, int z)
{
  values[calcIndex(x, y, z)] = std::move(layer);
}

void TileLayerTable::dispose(int x, int y, int z)
{
  int index = calcIndex(x, y, z);
  if (values[index] != nullptr) {
    values[index]->dispose();
    values[index].reset();
  }
}

int TileLayerTable::xSize() const
{
  return xSizeVal;
}

int TileLayerTable::ySize() const
{
  return ySizeVal;
}

int TileLayerTable::zSize() const
{
  return zSizeVal;
}

int TileLayerTable::size() const
{
  return sizeVal;
}

void TileLayerTable::resize(int x)
{
  resize(x, ySizeVal, zSizeVal);
}

void TileLayerTable::resize(int x, int y)
{
  resize(x, y, zSizeVal);
}

void TileLayerTable::resize(int x, int y, int z)
{
  if (x == xSizeVal && y == ySizeVal && z == zSizeVal) {
    return;
  }

  for (size_t i = 0; i < values.size(); ++i) {
    values[i]->dispose();
    values[i].reset();
  }

  size_t newSize = x * y * z;
  values.resize(newSize);
}

/*
    ⇩⇩⇩ Private ⇩⇩⇩
*/

inline const int TileLayerTable::calcIndex(int x, int y, int z) const
{
  return xSizeVal * ySizeVal * z + xSizeVal * y + x;
};

}  // namespace Eng