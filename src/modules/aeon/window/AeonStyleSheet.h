#pragma once

#include <SFML/System/Vector2.hpp>
#include <optional>

#include "AppDefs.h"
#include "engnine/Color.hpp"

namespace ae {

using app::String;
using Eng::Color;
using sf::Vector2f;

struct AeonStyleSheet {
  float ringSize;
  float ringOffset;
  float borderSize;
  float radius;
  float fontSize;
  Vector2f padding;
  String fontName;
  String ringColor;
  String borderColor;
  String bgColor;
  String textColor;
};

struct AeonPartialStyleSheet {
  std::optional<float> ringSize;
  std::optional<float> ringOffset;
  std::optional<float> borderSize;
  std::optional<float> radius;
  std::optional<float> fontSize;
  std::optional<Vector2f> padding;
  std::optional<String> fontName;
  std::optional<Color> ringColor;
  std::optional<Color> borderColor;
  std::optional<Color> bgColor;
  std::optional<Color> textColor;

  AeonPartialStyleSheet() = default;
  AeonPartialStyleSheet(const AeonStyleSheet& base);
};

extern AeonStyleSheet buttonDefaultStyle;

}  // namespace ae