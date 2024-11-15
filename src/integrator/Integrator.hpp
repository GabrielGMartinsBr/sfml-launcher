#pragma once

#include <ruby.h>

#include <boost/filesystem.hpp>
#include <cstdlib>
#include <stdexcept>

#include "It_Font.hpp"
#include "It_LoadSaveData.hpp"
#include "It_Window.hpp"
#include "integrator/It_Audio.hpp"
#include "integrator/It_Autotiles.hpp"
#include "integrator/It_Bitmap.hpp"
#include "integrator/It_Color.hpp"
#include "integrator/It_EngineLog.hpp"
#include "integrator/It_Graphics.hpp"
#include "integrator/It_Input.hpp"
#include "integrator/It_Plane.hpp"
#include "integrator/It_Rect.hpp"
#include "integrator/It_Sprite.hpp"
#include "integrator/It_Table.hpp"
#include "integrator/It_Tilemap.hpp"
#include "integrator/It_Tone.hpp"
#include "integrator/It_Viewport.hpp"
#include "aeon/AeonModuleIntegrator.h"

typedef VALUE (*Cb)(VALUE);
typedef boost::filesystem::path FPath;

struct Integrator {
  void init()
  {
    if (initialized) {
      throw std::runtime_error("Can not initialize Integrator twice.");
    }

    ruby_init();
    // ruby_init_loadpath();

    It::EngineLog::integrate();
    It::LoadSaveData::integrate();
    It::Graphics::integrate();
    It::Audio::integrate();
    It::Input::integrate();
    It::Color::integrate();
    It::Tone::integrate();
    It::Rect::integrate();
    It::Viewport::integrate();
    It::Bitmap::integrate();
    It::Sprite::integrate();
    It::Window::integrate();
    It::Font::integrate();
    It::Table::integrate();
    It::Tilemap::integrate();
    It::Autotiles::integrate();
    It::Plane::integrate();

    ae::AeonModuleIntegrator::integrate();

    loadScriptsPath();
    loadBuiltInScripts();

    // require("script_05.rb");

    initialized = true;
  }

  void cleanup()
  {
    // std::cout << "[Ruby]: Finalized\n";
    ruby_cleanup(0);
  }

 private:
  bool initialized = false;

  void loadBuiltInScripts()
  {
    require("built-in/RGSS-BuiltIn.rb");
  }

  void loadScriptsPath()
  {
    FPath relativePath("/run/media/home/common/gabrielmartins.dev/dev/cpp/orm-xp/sfml-launcher/scripts");
    FPath absolutePath = boost::filesystem::canonical(relativePath);
    const char* scriptDirPath = absolutePath.c_str();

    VALUE loadPathArray = rb_gv_get("$LOAD_PATH");
    rb_funcall(loadPathArray, rb_intern("<<"), 1, rb_str_new2(scriptDirPath));
  }

  void require(const char* c)
  {
    int error;
    rb_protect((Cb)RUBY_METHOD_FUNC(rb_require), (VALUE)c, &error);
    if (error) {
      VALUE error_message = rb_gv_get("$!");
      rb_p(error_message);
      std::exit(1);
    }
  }
};