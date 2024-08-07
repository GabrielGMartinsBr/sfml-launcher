#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <boost/asio/io_context.hpp>

#include "base/AppDefs.h"
#include "debugger/Debugger.h"
#include "launcher/Launcher.hpp"

int main(int argc, char** argv)
{
  Launcher launcher;
  Debugger::Debugger& debugger = Debugger::Debugger::getInstance();

  debugger.start();

  if (argc == 2) {
    launcher.run(argv[1]);
  } else {
    // app::CStr devScriptPath = "../GameData/projects/project-01/Scripts.rxdata";
    // app::CStr devScriptPath = "../GameData/projects/project-02/Scripts.rxdata";
    // app::CStr devScriptPath = "../../projects/FullScripts.rxdata";
    // app::CStr devScriptPath = "../../projects/dev-project-graphics/Data/Scripts.rxdata";
    // app::CStr devScriptPath = "../../projects/Project1/Data/Scripts.rxdata";
    app::CStr devScriptPath = "../../projects/Project1";
    launcher.run(devScriptPath);
  }

  debugger.serverThread->join();

  return 0;
}