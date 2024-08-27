#pragma once

#include <SFML/Graphics/Font.hpp>
#include <unordered_map>

#include "AppDefs.h"
#include "engnine/EngineBase.hpp"

namespace Eng {

using app::CStr;
using app::String;

struct Fonts {
  /*
    ⇩⇩⇩ Static ⇩⇩⇩
  */

  static void Init();
  static Fonts& Instance();
  static void Destroy();

  /*
    ⇩⇩⇩ Instance ⇩⇩⇩
  */


  const sf::Font* getFont(const String& name);


 private:
  /*
    ⇩⇩⇩ Private ⇩⇩⇩
  */

  std::unordered_map<String, sf::Font> fontMap;

  Fonts();

  Fonts(const Fonts&);
  Fonts& operator=(const Fonts&);

  void loadFonts();

  void loadFont(String name, CStr path);



};

}  // namespace Eng