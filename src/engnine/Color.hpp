#pragma once

// TODO: Add values range automatically correction

class EngColor {
 public:
  int red = 0;
  int green = 0;
  int blue = 0;

  EngColor(int r, int g, int b)
  {
    set(r, g, b);
  }

  void set(int r, int g, int b)
  {
    red = r;
    green = g;
    blue = b;
  }
};