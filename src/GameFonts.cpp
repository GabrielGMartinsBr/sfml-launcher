#include "GameFonts.h"

sf::Font GameFonts::defaultFont; 

void GameFonts::load()
{
  if (!defaultFont.loadFromFile("../assets/font.ttf")) {
    throw;
  }
}