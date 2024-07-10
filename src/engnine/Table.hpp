#pragma once

#include "base/AppDefs.h"
#include "base/Log.hpp"

namespace Eng {

class Table {
 public:

  Table(int x, int y = 1, int z = 1) :
      xSize(x),
      ySize(y),
      zSize(z),
      values(x * y * z) { }

  // Clone another instance
  Table(const Table &source) :
      xSize(source.xSize),
      ySize(source.ySize),
      zSize(source.zSize),
      values(source.values) { }

  inline const short getValue(int x, int y = 0, int z = 0) const
  {
    return values[calcIndex(x, y, z)];
  }

  inline const void setValue(short value, int x, int y = 0, int z = 0)
  {
    if (x < 0 || x >= xSize) {
      Log::err() << "x index is out of range.";
      return;
    }
    if (y < 0 || y >= ySize) {
      Log::err() << "y index is out of range.";
      return;
    }
    if (z < 0 || z >= zSize) {
      Log::err() << "z index is out of range.";
      return;
    }
    values[calcIndex(x, y, z)] = value;
  }

  inline const int getXSize() const
  {
    return xSize;
  }

  inline const int getYSize() const
  {
    return ySize;
  }

  inline const int getZSize() const
  {
    return zSize;
  }

  void resize(int x, int y, int z)
  {
    if (x == xSize && y == ySize && z == zSize) {
      return;
    }

    app::Vector<short> newValues(xSize * ySize * zSize);

    int xLimit = std::min(x, xSize);
    int yLimit = std::min(y, ySize);
    int zLimit = std::min(z, zSize);
    int index;

    for (int _z = 0; _z < zLimit; ++_z) {
      for (int _y = 0; _y < yLimit; ++_y) {
        for (int _x = 0; _x < xLimit; ++_x) {
          index = calcIndex(_x, _y, _z);
          newValues[index] = values[index];
        }
      }
    }

    values.swap(newValues);

    xSize = x;
    ySize = y;
    zSize = z;
  }

  void resize(int x, int y)
  {
    resize(x, y, zSize);
  }

  void resize(int x)
  {
    resize(x, ySize, zSize);
  }


 private:
  int xSize;
  int ySize;
  int zSize;
  app::Vector<short> values;

  inline const int calcIndex(int x, int y = 0, int z = 0) const
  {
    return xSize * ySize * z + xSize * y + x;
  };
};

}  // namespace Eng