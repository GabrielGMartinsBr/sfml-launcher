#include "Fonts.h"

#include <cassert>

#include "AppDefs.h"
#include "Log.hpp"
#include "engnine/FileUtils.hpp"

namespace Eng {

using app::FilePath;

static CStr fontDir = "/run/media/home/common/gabrielmartins.dev/dev/cpp/orm-xp/sfml-launcher/assets";

/*
  ⇩⇩⇩ Static ⇩⇩⇩
*/

static Fonts* instance = nullptr;

void Fonts::Init()
{
  assert(!instance);
  instance = new Fonts();
  instance->loadFonts();
}

Fonts& Fonts::Instance()
{
  assert(instance);
  return *instance;
}

void Fonts::Destroy()
{
  assert(instance);
  delete instance;
  instance = nullptr;
}

/*
  ⇩⇩⇩ Instance ⇩⇩⇩
*/

Fonts::Fonts() { }

const sf::Font* Fonts::getFont(const String& name)
{
  if (fontMap.find(name) == fontMap.end()) {
    return nullptr;
  }

  return &fontMap[name];
}

void Fonts::loadFonts()
{
  loadFont("Arial", "Arial.ttf");
  loadFont("Arial-medium", "Arial-medium.ttf");
  loadFont("Arial-bold", "Arial-bold.ttf");
  loadFont("Arial-bold-ce", "arialceb.ttf");
  loadFont("Roboto", "Roboto-medium.ttf");
}

void Fonts::loadFont(String name, CStr fileName)
{
  FilePath path = FileUtils::combine(fontDir, fileName);

  sf::Font font;
  if (!font.loadFromFile(path.c_str())) {
    Log::err() << "Failed to load font";
    return;
  }

  fontMap[name] = font;
  // Log::out() << "Font '" << name << "' was loaded!";
}

}  // namespace Eng