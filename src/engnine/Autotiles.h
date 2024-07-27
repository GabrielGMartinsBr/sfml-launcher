#pragma once

#include "engnine/Bitmap.h"
#include "engnine/EngineBase.hpp"

namespace Eng {

class Autotiles : public EngineBase {
 public:

  Autotiles();
  ~Autotiles();

  void bindRubyProps();

  bool setter(int index, Bitmap* value);
  Bitmap* getter(int index);

 private:

  Bitmap* bitmaps[6];
};

}  // namespace Eng