#pragma once

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <stdexcept>

#include "base/AppDefs.h"
#include "engnine/Font.hpp"

namespace Eng {

class Texts {


 public:
  static sf::Font font;

  static void loadFonts()
  {
    static sf::Font font;
    if (!font.loadFromFile("../assets/arial.ttf")) {
      throw std::runtime_error("Load fonts failed");
    }
  }

  static void loadFont(sf::Font &font)
  {
    if (!font.loadFromFile("../assets/arial.ttf")) {
      throw std::runtime_error("Load fonts failed");
    }
  }

  static void drawText(sf::RenderTexture &texture, int x, int y, Eng::Font &font, const app::CStr &str)
  {
    sf::Font sfFont;
    loadFont(sfFont);
    sfFont.setSmooth(false);
    sf::Text text;
    text.setFont(sfFont);
    text.setString(str);
    text.setCharacterSize(font.getter_size());
    text.setFillColor(font.getter_color()->getSfColor());
    text.setPosition(x, y);
    text.setOutlineColor(sf::Color::White);
    // text.setOutlineThickness(.5);
    // texture.setSmooth(true);
    texture.draw(text, sf::BlendNone);
  }
};

}  // namespace Eng