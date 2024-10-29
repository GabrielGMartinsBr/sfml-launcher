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

using dbg::Debugger;

int main(int argc, char** argv)
{
  Launcher launcher;
  Debugger& debugger = Debugger::getInstance();

  if (argc == 2) {
    launcher.run(argv[1]);
  } else if (argc == 3) {
    if (strcmp(argv[1], "debug") == 0) {
      debugger.start();
      launcher.run(argv[2]);
    } else {
      Log::err() << "Invalid arguments.";
      return 1;
    }
  } else if (argc == 1) {
    // app::CStr devScriptPath = "../../projects/Project1";
    // app::CStr devScriptPath = "../../projects/dev-project-graphics";
    // app::CStr devScriptPath = "../../projects/Project2";
    app::CStr devScriptPath = "/run/media/home/common/gabrielmartins.dev/dev/cpp/orm-xp/projects/aeon/Old Aeon Netplay";
    launcher.run(devScriptPath);
  }

  if (debugger.isRunning()) {
    debugger.stop();
    debugger.serverThread->join();
  }

  return 0;
}