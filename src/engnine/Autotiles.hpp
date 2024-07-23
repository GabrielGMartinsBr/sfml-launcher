#pragma once

#include "engnine/Bitmap.hpp"
#include "engnine/EngineBase.hpp"

namespace Eng {

class Autotiles : public EngineBase {
 public:

  Autotiles()
  {
  }

  bool setter(int index, Bitmap* value)
  {
    if (index < 0 && index > 6) {
      return false;
    }

    bitmaps[index] = value;

    return true;
  }

  Bitmap* getter(int index)
  {
    if (index < 0 && index > 6) {
      return nullptr;
    }
    return bitmaps[index];
  }

 private:
  Bitmap* bitmaps[6];
};

}  // namespace Eng