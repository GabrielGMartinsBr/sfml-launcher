#pragma once

#include <boost/filesystem.hpp>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "It_LoadSaveData.hpp"
#include "integrator/It_Bitmap.hpp"
#include "integrator/It_Color.hpp"
#include "integrator/It_Graphics.hpp"
#include "integrator/It_Input.hpp"
#include "integrator/It_Rect.hpp"
#include "integrator/It_Sprite.hpp"
#include "integrator/It_Table.hpp"
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
    // ruby_init_loadpath();

    loadScriptsPath();

    It::Color::integrate();
    It::Tone::integrate();
    It::Rect::integrate();
    It::Viewport::integrate();
    It::Bitmap::integrate();
    It::Sprite::integrate();
    It::Graphics::integrate();
    It::LoadSaveData::integrate();

    loadBuiltInScripts();

    input.integrate();

    // require("script_05.rb");

    initialized = true;
  }

  void cleanup()
  {
    std::cout << "[Ruby]: Finalized\n";
    ruby_cleanup(0);
  }

 private:

  void loadBuiltInScripts()
  {
    require("built-in/RPG::Cache.rb");
    require("built-in/RPG::Sprite.rb");
    require("built-in/RPG::Weather.rb");
    require("built-in/RPG::Map.rb");
    require("built-in/RPG::MapInfo.rb");
    require("built-in/RPG::Event.rb");
    require("built-in/RPG::Event::Page.rb");
    require("built-in/RPG::Event::Page::Condition.rb");
    require("built-in/RPG::Event::Page::Graphic.rb");
    require("built-in/RPG::EventCommand.rb");
    require("built-in/RPG::MoveRoute.rb");
    require("built-in/RPG::MoveCommand.rb");
    require("built-in/RPG::Actor.rb");
    require("built-in/RPG::Class.rb");
    require("built-in/RPG::Class::Learning.rb");
    require("built-in/RPG::Skill.rb");
    require("built-in/RPG::Item.rb");
    require("built-in/RPG::Weapon.rb");
    require("built-in/RPG::Armor.rb");
    require("built-in/RPG::Enemy.rb");
    require("built-in/RPG::Enemy::Action.rb");
    require("built-in/RPG::Troop.rb");
    require("built-in/RPG::Troop::Member.rb");
    require("built-in/RPG::Troop::Page.rb");
    require("built-in/RPG::Troop::Page::Condition.rb");
    require("built-in/RPG::State.rb");
    require("built-in/RPG::Animation.rb");
    require("built-in/RPG::Animation::Frame.rb");
    require("built-in/RPG::Animation::Timing.rb");
    require("built-in/RPG::Tileset.rb");
    require("built-in/RPG::CommonEvent.rb");
    require("built-in/RPG::System.rb");
    require("built-in/RPG::System::Words.rb");
    require("built-in/RPG::AudioFile.rb");

  }

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