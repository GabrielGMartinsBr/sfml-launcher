#pragma once

#include "base/Log.hpp"

namespace Eng {

class Audio {

 public:
  static void bgm_play()
  {
    Log::out() << "Play bgm was called, but it is not implemented yet.";
  }

  static void bgm_stop()
  {
    Log::out() << "Stop bgm was called, but it is not implemented yet.";
  }
};
}  // namespace Eng