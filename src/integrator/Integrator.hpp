#pragma once

#include <boost/filesystem.hpp>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "integrator/It_Bitmap.hpp"
#include "integrator/It_Color.hpp"
#include "integrator/It_Graphics.hpp"
#include "integrator/It_Input.hpp"
#include "integrator/It_Rect.hpp"
#include "integrator/It_Sprite.hpp"
#include "integrator/It_Tone.hpp"
#include "integrator/It_Viewport.hpp"
#include "ruby.h"

typedef VALUE (*Cb)(VALUE);
typedef boost::filesystem::path FPath;

class Integrator {
  bool initialized = false;

  It::Input input;

 public:

  void init()
  {
    if (initialized) {
      throw std::runtime_error("Can not initialize Integrator twice.");
    }

    std::cout << "[Ruby]: Init\n";

    ruby_init();

    loadScriptsPath();

    It::Color::integrate();
    It::Tone::integrate();
    It::Rect::integrate();
    It::Viewport::integrate();
    It::Bitmap::integrate();
    It::Sprite::integrate();
    It::Graphics::integrate();

    input.integrate();

    // require("script_05.rb");

    initialized = true;
  }

  void cleanup()
  {
    std::cout << "[Ruby]: Finalized\n";
    ruby_finalize();
  }

 private:

  void loadScriptsPath()
  {
    FPath relativePath("../scripts");
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