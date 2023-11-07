#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

#include "GameInput.h"
#include "launcher/Launcher.hpp"

GameInput game_input;

int main(int, char **)
{
  // RubyService rService;
  // rService.run();

  Launcher launcher;

  launcher.run();
}