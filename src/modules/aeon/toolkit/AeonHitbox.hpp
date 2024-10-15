#pragma once

namespace ae {

struct AeonHitBox {
  AeonHitBox() :
      AeonHitBox(0, 0, 0, 0) { }

  AeonHitBox(float x, float y, float width, float height) :
      startX(x),
      startY(y),
      width(width),
      height(height),
      endX(x + width),
      endY(y + height)
  {
  }

  bool intersects(float x, float y)
  {
    return !isEmpty() && x >= startX && x <= endX && y >= startY && y < endY;
  }

  bool isEmpty()
  {
    return width == 0 || height == 0;
  }

  void updateX(float value)
  {
    startX = value;
    endX = startX + width;
  }

  void updateY(float value)
  {
    startY = value;
    endY = startY + height;
  }

  void updateWidth(float value)
  {
    width = value;
    endX = startX + width;
  }

  void updateHeight(float value)
  {
    height = value;
    endY = startY + height;
  }

 private:
  float startX;
  float startY;
  float width;
  float height;
  float endX;
  float endY;
};

}  // namespace ae