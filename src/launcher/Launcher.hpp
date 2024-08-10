#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <sstream>
#include <string>

#include "Log.hpp"
#include "base/AppDefs.h"
#include "base/BacktraceUtils.hpp"
#include "engnine/Engine.h"
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

  void run(app::CStr projectPath)
  {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::Vector2i winPos(
      (desktop.width - width) / 2,
      (desktop.height - height) / 2
    );
    window.setPosition(winPos);

    ScriptsLoader& scriptsLoader = ScriptsLoader::getInstance();

    engine.init(window, projectPath);
    integrator.init();

    app::String scriptsPath = engine.getScriptsPath();
    loadScripts(scriptsPath.c_str());

    integrator.cleanup();
    window.close();
  }

  void loadScripts(app::CStr scriptsPath)
  {
    app::Vector<PlayerScript> scripts;
    ScriptsLoader::getInstance().loadScripts(
      scripts,
      scriptsPath
    );

    int errorState;
    VALUE rubyError;
    VALUE result;

    std::string code;
    app::ULong lineNumber = 0;

    for (PlayerScript& script : scripts) {
      code.append(script.code).append("\n");
      script.setStartLine(lineNumber);
      lineNumber = script.getEndLine();
    }

    // findErrorLine(scripts, 6991);

    rb_eval_string_protect(code.c_str(), &errorState);

    if (errorState) {
      logError(scripts);
      // throwErrorOnScript(script);
    }
  }

  void findErrorLine(const app::Vector<PlayerScript>& scripts, int lineNumber)
  {
    const PlayerScript* script = getScriptLine(scripts, lineNumber);
    if (script != nullptr) {
      lineNumber -= script->getStartLine();
      Log::out() << "Name: " << script->name;
      Log::out() << "line: " << lineNumber;
    }
  }

  void logError(const app::Vector<PlayerScript>& scripts)
  {
    VALUE errinfo = ruby_errinfo;
    ruby_errinfo = Qnil;
    rb_p(errinfo);

    VALUE backtrace = rb_funcall(errinfo, rb_intern("backtrace"), 0);
    VALUE err_message = rb_funcall(errinfo, rb_intern("message"), 0);
    VALUE err_class = rb_class_of(errinfo);

    // const char* class_name = rb_class2name(err_class);
    // Log::out() << "class_name: " << class_name;
    // rb_p(backtrace);

    VALUE backtrace_0 = rb_ary_entry(backtrace, 0);
    const char* backtrace_0_str = StringValueCStr(backtrace_0);

    long lineNumber;
    BacktraceUtils::pickLineNumber(backtrace_0_str, lineNumber);
    const PlayerScript* script = getScriptLine(scripts, lineNumber);
    if (script == nullptr) {
      return;
    }
    long scriptLine = lineNumber - script->getStartLine();

    std::string cleanErrorMessage;
    std::string message = StringValueCStr(err_message);
    // Log::out() << "err_message: " << message;

    BacktraceUtils::pickMessage(message, cleanErrorMessage);
    // Log::out() << "err_message: " << cleanErrorMessage;

    Log::err() << "An error has occurred on script '"
               << script->name << "' at line " << scriptLine << ": " << cleanErrorMessage;

    VALUE err_class_path = rb_class_path(rb_obj_class(errinfo));

    Log::err() << "\t<RuntimeError>";
    Log::err() << "\t\tClass: " << StringValueCStr(err_class_path);
    Log::err() << "\t\tMessage: " << cleanErrorMessage;

    logBacktrace(scripts, backtrace);

    Log::err() << "\t</RuntimeError>";
  }

  void logBacktrace(const app::Vector<PlayerScript>& scripts, const VALUE backtrace)
  {
    int length = RARRAY_LEN(backtrace);
    VALUE backtraceEntry;
    const char* backtrace_str;
    const PlayerScript* script;
    long lineNumber;
    long scriptLine;
    std::string methodStr;
    Log::err() << "\t\t<ExceptionBacktrace>";
    for (int i = 0; i < length; i++) {
      backtraceEntry = rb_ary_entry(backtrace, i);
      backtrace_str = StringValueCStr(backtraceEntry);
      BacktraceUtils::pickLineNumber(backtrace_str, lineNumber);
      BacktraceUtils::pickMethod(backtrace_str, methodStr);
      script = getScriptLine(scripts, lineNumber);
      if (script == nullptr) {
        continue;
      }
      scriptLine = lineNumber - script->getStartLine();
      Log::err() << "\t\t\t" << script->id << ":" << script->name << ":" << scriptLine << methodStr;
    }
    Log::err() << "\t\t</ExceptionBacktrace>";
  }

  const PlayerScript* getScriptLine(const app::Vector<PlayerScript>& scripts, long lineNumber)
  {
    for (const PlayerScript& script : scripts) {
      if (lineNumber > script.getEndLine()) {
        continue;
      }
      if (lineNumber >= script.getStartLine() && lineNumber <= script.getEndLine()) {
        return &script;
      }
    }
    return nullptr;
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