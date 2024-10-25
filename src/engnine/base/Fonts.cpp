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
  loadFont("Arial Medium", "Arial-Medium.ttf");
  loadFont("Arial Bold", "Arial-Bold.ttf");
  loadFont("Arial Black", "Arial-Black.ttf");
  loadFont("Arial Bold Ce", "Arial-Bold-Ce.ttf");
  loadFont("Roboto Thin", "Roboto-Thin.ttf");
  loadFont("Roboto Light", "Roboto-Light.ttf");
  loadFont("Roboto", "Roboto-Regular.ttf");
  loadFont("Roboto Medium", "Roboto-Medium.ttf");
  loadFont("Roboto Bold", "Roboto-Bold.ttf");
  loadFont("Roboto Black", "Roboto-Black.ttf");
  loadFont("Tahoma", "Tahoma.ttf");
  loadFont("Tahoma Bold", "Tahoma-Bold.ttf");
  

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