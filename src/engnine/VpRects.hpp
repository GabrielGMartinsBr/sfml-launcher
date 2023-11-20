#pragma once

namespace vp {

struct ViewportRect {
  int x = 0;
  int y = 0;
  int width = 0;
  int height = 0;
  int endX() { return x + width; }
  int endY() { return y + height; }
};

struct DestinyRect {
  int x = 0;
  int y = 0;
  int width = 0;
  int height = 0;
  int left = 0;
  int top = 0;
};

}  // namespace vp