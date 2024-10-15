#pragma once

#include <SFML/System/Vector2.hpp>

#include "AppDefs.h"
#include "engnine/Color.hpp"

namespace ae {

using app::String;
using Eng::Color;
using sf::Vector2f;

struct AeonStylesheet {
  float ringSize;
  float ringOffset;
  float borderSize;
  float radius;
  float fontSize;
  Vector2f padding;
  String fontName;
  Color ringColor;
  Color borderColor;
  Color bgColor;
  Color textColor;

  AeonStylesheet();
};

}  // namespace ae