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
#include "base/AppDefs.h"
#include "base/Log.hpp"
#include "launcher/Launcher.hpp"

GameInput game_input;

int main(int argc, char** argv)
{
  Launcher launcher;

  Log::out() << argc;

  if (argc == 2) {
    launcher.run(argv[1]);
  } else {
    app::CStr devScriptPath = "../GameData/projects/project-01/Scripts.rxdata";
    launcher.run(devScriptPath);
  }
}