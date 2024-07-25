#pragma once

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Utf.hpp>
#include <stdexcept>

#include "Log.hpp"
#include "base/AppDefs.h"
#include "engnine/Font.hpp"

namespace Eng {

class Texts {


 public:
  static sf::Font font;

  static void loadFonts()
  {
    static sf::Font font;
    if (!font.loadFromFile("/run/media/home/common/gabrielmartins.dev/dev/cpp/orm-xp/sfml-launcher/assets/arial.ttf")) {
      throw std::runtime_error("Load fonts failed");
    }
  }

  static void loadFont(sf::Font &font)
  {
    if (!font.loadFromFile("/run/media/home/common/gabrielmartins.dev/dev/cpp/orm-xp/sfml-launcher/assets/arial.ttf")) {
      throw std::runtime_error("Load fonts failed");
    }
  }

  static void drawText(sf::RenderTexture &texture, int x, int y, Eng::Font &font, const app::CStr &str)
  {
    sf::String s = fromUtf8(str);
    sf::Font sfFont;
    loadFont(sfFont);
    sfFont.setSmooth(false);
    sf::Text text;
    text.setFont(sfFont);
    // text.setString(str);
    text.setString(s);
    text.setCharacterSize(font.getter_size());
    text.setFillColor(font.getter_color()->getSfColor());
    text.setPosition(x, y);
    text.setOutlineColor(sf::Color::White);
    // text.setOutlineThickness(.5);
    // texture.setSmooth(true);
    texture.draw(text, sf::BlendNone);
  }

  static sf::String fromUtf8(const std::string &in)
  {
    std::basic_string<sf::Uint32> tmp;
    sf::Utf8::toUtf32(in.begin(), in.end(), back_inserter(tmp));
    return sf::String(tmp);
  }

  static std::string toUtf8(const sf::String &str)
  {
    std::string out;
    sf::Utf32::toUtf8(str.begin(), str.end(), back_inserter(out));
    return out;
  }
};

}  // namespace Eng