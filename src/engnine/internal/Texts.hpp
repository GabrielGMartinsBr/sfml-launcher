#pragma once

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Utf.hpp>

#include "base/AppDefs.h"

namespace Eng {

using app::CStr;

struct Texts {
  static sf::Text createText(CStr str)
  {
    sf::String s = fromUtf8(str);
    sf::Text text;
    text.setString(s);
    return text;
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