#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <sstream>

#include "engnine/Engine.hpp"
#include "integrator/Integrator.hpp"
#include "loaders/PlayerScript.hpp"
#include "loaders/ScriptsLoader.hpp"

class Launcher {
  int width = 640;
  int height = 480;
  const char* title = "ORM S-Launcher";

  sf::VideoMode mode;
  sf::RenderWindow window;

  Integrator integrator;

  Eng::Engine& engine = Eng::Engine::getInstance();

 public:

  Launcher() :
      mode(width, height),
      window(mode, title) { }

  void run()
  {
    ScriptsLoader& scriptsLoader = ScriptsLoader::getInstance();

    engine.init(window);
    integrator.init();

    loadScripts();

    integrator.cleanup();
    window.close();
  }

  void loadScripts()
  {
    app::Vector<PlayerScript> scripts;
    ScriptsLoader::getInstance().loadScripts(
      scripts,
      "../GameData/projects/project-01/Scripts.rxdata"
    );

    VALUE rubyError;

    for (const PlayerScript& script : scripts) {
      rb_eval_string(
        script.code.c_str()
      );

      if (hasRubyError(rubyError)) {
        throwErrorOnScript(script);
      }

      Log::out() << "Script \"" << script.name << "\" loaded!";
    }
  }

  bool hasRubyError(VALUE& rubyError)
  {
    rubyError = rb_gv_get("$!");
    if (rubyError != Qnil) {
      rb_p(rubyError);
      return true;
    }
    return false;
  }

  void throwErrorOnScript(const PlayerScript& script)
  {
    std::stringstream errorMsg;
    errorMsg << "Error on script with id: ";
    errorMsg << script.id;
    errorMsg << " and name: ";
    errorMsg << script.name;
    throw std::runtime_error(errorMsg.str());
  }
};