#pragma once

#include <vector>

#include "ruby.h"

struct GlobalTest {
  std::vector<VALUE> sprites;

  GlobalTest();

  void addSprite(VALUE ptr);
};

extern GlobalTest globalInst;
