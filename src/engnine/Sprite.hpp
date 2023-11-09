#pragma once

#include "engnine/Bitmap.hpp"
#include "engnine/Color.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Tone.hpp"
#include "ruby.h"

namespace Eng {

class Sprite {

 public:
  Bitmap *bitmap = nullptr;
  VALUE bitmap_ptr;
  Rect src_rect;
  bool visible;
  int x;
  int y;
  int z;
  int ox;
  int oy;
  int zoom_x;
  int zoom_y;
  int angle;
  int mirror;
  int bush_depth;
  int opacity;
  int blend_type;
  Color color;
  Tone tone;

  Sprite() :
      color(0, 0, 0),
      tone(0, 0, 0)
  {
  }

  void dispose() { }

  void disposed() { }

  void viewport() { }

  void update() { }
};

}  // namespace Eng