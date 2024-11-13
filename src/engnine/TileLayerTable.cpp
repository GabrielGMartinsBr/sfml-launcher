#include "engnine/TileLayerTable.h"

#include <memory>

namespace Eng {

/*
    ⇩⇩⇩ Public ⇩⇩⇩
*/

TileLayerTable::TileLayerTable(int x, int y) :
    xSizeVal(x),
    ySizeVal(y),
    sizeVal(x * y),
    values(sizeVal)
{
}

const UPtr<TilemapLayer>& TileLayerTable::operator[](size_t index) const
{
  return values[index];
}

bool TileLayerTable::isEmpty(int x, int y)
{
  return values[calcIndex(x, y)] == nullptr;
}

TilemapLayer& TileLayerTable::getValue(int x, int y)
{
  return *values[calcIndex(x, y)];
}

void TileLayerTable::setValue(UPtr<TilemapLayer> layer, int x, int y)
{
  values[calcIndex(x, y)] = std::move(layer);
}

void TileLayerTable::dispose(int x, int y)
{
  int index = calcIndex(x, y);
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

int TileLayerTable::size() const
{
  return sizeVal;
}

void TileLayerTable::resize(int x)
{
  resize(x, ySizeVal);
}

void TileLayerTable::resize(int x, int y)
{
  if (x == xSizeVal && y == ySizeVal) {
    return;
  }
  xSizeVal = x;
  ySizeVal = y;
  sizeVal = x * y;

  for (size_t i = 0; i < values.size(); ++i) {
    values[i]->dispose();
    values[i].reset();
  }

  size_t newSize = x * y;
  values.resize(newSize);
}

/*
    ⇩⇩⇩ Private ⇩⇩⇩
*/

inline const int TileLayerTable::calcIndex(int x, int y) const
{
  return xSizeVal * y + x;
};

}  // namespace Eng