#pragma once

#include <SFML/Config.hpp>
#include <array>

#include "AppDefs.h"
#include "engnine/Color.hpp"

namespace ae {

using app::String;

typedef std::array<sf::Uint8, 4> Uint8RgbaArray;
typedef std::array<float, 4> FloatRgbaArray;

struct ColorParser {
  static Eng::Color hexToNrgssColor(const String& hexColor)
  {
    Uint8RgbaArray rgba = hexToRGBA(hexColor);
    return Eng::Color(rgba[0], rgba[1], rgba[2], rgba[3]);
  }

  static sf::Color hexToSfmlColor(const String& hexColor)
  {
    Uint8RgbaArray rgba = hexToRGBA(hexColor);
    return sf::Color(rgba[0], rgba[1], rgba[2], rgba[3]);
  }

  static Eng::Color floatToNrgssColor(const FloatRgbaArray& rgba)
  {
    return Eng::Color(rgba[0] * 255, rgba[1] * 255, rgba[2] * 255, rgba[3] * 255);
  }

  static sf::Color floatToSfmlColor(const FloatRgbaArray& rgba)
  {
    return sf::Color(rgba[0] * 255, rgba[1] * 255, rgba[2] * 255, rgba[3] * 255);
  }

  static Uint8RgbaArray hexToRGBA(const String& hexColor)
  {
    if (hexColor[0] != '#') {
      throw std::invalid_argument("Color must start with #.");
    }

    size_t len = hexColor.length();
    Uint8RgbaArray rgba = { 255, 255, 255, 255 };  // Default to white and alpha 255

    std::stringstream ss;

    // Process 3 or 4 character hex strings (#RGB or #RGBA)
    if (len == 4 || len == 5) {
      for (size_t i = 1; i < len; ++i) {
        ss.clear();
        ss << std::hex << hexColor[i] << hexColor[i];  // Double each character
        int value;
        ss >> value;
        rgba[i - 1] = static_cast<uint8_t>(value);
      }
      if (len == 4) {
        rgba[3] = 255;  // Omitted alpha
      }
    }
    // Process 6 or 8 character hex strings (#RRGGBB or #RRGGBBAA)
    else if (len == 7 || len == 9) {
      for (size_t i = 1; i < len; i += 2) {
        ss.clear();
        ss << std::hex << hexColor.substr(i, 2);
        int value;
        ss >> value;
        rgba[(i - 1) / 2] = static_cast<uint8_t>(value);
      }
      if (len == 7) {
        rgba[3] = 255;  // Omitted alpha
      }
    } else {
      throw std::invalid_argument("Invalid hex color format.");
    }

    return rgba;
  }
};

}  // namespace ae