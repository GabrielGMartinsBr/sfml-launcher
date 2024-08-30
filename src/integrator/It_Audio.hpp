#pragma once

#include <stdexcept>
#include <string>

#include "AppDefs.h"
#include "engnine/Audio.h"
#include "integrator/Convert.hpp"
#include "ruby.h"

namespace It {

using app::CStr;

class Audio {
 public:
  static void integrate()
  {
    VALUE audioModule = rb_define_module("Audio");

    rb_define_module_function(audioModule, "bgm_play", RUBY_METHOD_FUNC(bgm_play), -1);
    rb_define_module_function(audioModule, "bgm_stop", RUBY_METHOD_FUNC(bgm_stop), 0);
    rb_define_module_function(audioModule, "bgm_fade", RUBY_METHOD_FUNC(bgm_fade), 1);

    rb_define_module_function(audioModule, "bgs_play", RUBY_METHOD_FUNC(bgs_play), -1);
    rb_define_module_function(audioModule, "bgs_stop", RUBY_METHOD_FUNC(bgs_stop), 0);
    rb_define_module_function(audioModule, "bgs_fade", RUBY_METHOD_FUNC(bgs_fade), 1);

    rb_define_module_function(audioModule, "me_play", RUBY_METHOD_FUNC(me_play), -1);
    rb_define_module_function(audioModule, "me_stop", RUBY_METHOD_FUNC(me_stop), 0);
    rb_define_module_function(audioModule, "me_fade", RUBY_METHOD_FUNC(me_fade), 1);

    rb_define_module_function(audioModule, "se_play", RUBY_METHOD_FUNC(se_play), -1);
    rb_define_module_function(audioModule, "se_stop", RUBY_METHOD_FUNC(se_stop), 0);
    rb_define_module_function(audioModule, "se_fade", RUBY_METHOD_FUNC(se_fade), 1);
  }

 private:

  /*
    BGM
  */

  static VALUE bgm_play(int argc, VALUE *argv, VALUE self)
  {
    VALUE _filename, _volume, _pitch;
    switch (argc) {
      case 1: {
        rb_scan_args(argc, argv, "1", &_filename);
        Eng::Audio::Instance().bgm_play();
        break;
      }
      case 2: {
        rb_scan_args(argc, argv, "2", &_filename, &_volume);
        Eng::Audio::Instance().bgm_play();
        break;
      }
      case 3: {
        rb_scan_args(argc, argv, "3", &_filename, &_volume, &_pitch);
        Eng::Audio::Instance().bgm_play();
        break;
      }
      default: {
        throw std::runtime_error("Audio.bgm_play takes 1 to 3 arguments, but were received " + std::to_string(argc));
      }
    }
    return Qnil;
  }

  static VALUE bgm_stop(VALUE self)
  {
    Eng::Audio::Instance().bgm_stop();
    return Qnil;
  }

  static VALUE bgm_fade(VALUE self, VALUE _time)
  {
    int time = Convert::toCInt(_time);

    Eng::Audio::Instance().bgm_fade(time);
    return Qnil;
  }

  /*
    BGS
  */

  static VALUE bgs_play(int argc, VALUE *argv, VALUE self)
  {
    CStr fileName = Convert::toCStr(argv[0]);
    switch (argc) {
      case 1: {
        Eng::Audio::Instance().bgs_play(fileName);
        break;
      }
      case 2: {
        float volume = Convert::toCDouble2(argv[1]);
        Eng::Audio::Instance().bgs_play(fileName, volume);
        break;
      }
      case 3: {
        float volume = Convert::toCDouble2(argv[1]);
        float pitch = Convert::toCDouble2(argv[2]);
        Eng::Audio::Instance().bgs_play(fileName, volume, pitch / 100);
        break;
      }
      default: {
        throw std::runtime_error("Audio.bgm_play takes 1 to 3 arguments, but were received " + std::to_string(argc));
      }
    }
    return Qnil;
  }

  static VALUE bgs_stop(VALUE self)
  {
    Eng::Audio::Instance().bgs_stop();
    return Qnil;
  }

  static VALUE bgs_fade(VALUE self, VALUE _time)
  {
    int time = Convert::toCInt(_time);

    Eng::Audio::Instance().bgs_fade(time);
    return Qnil;
  }

  /*
    ME
  */

  static VALUE me_play(int argc, VALUE *argv, VALUE self)
  {
    VALUE _filename, _volume, _pitch;
    switch (argc) {
      case 1: {
        rb_scan_args(argc, argv, "1", &_filename);
        Eng::Audio::Instance().me_play();
        break;
      }
      case 2: {
        rb_scan_args(argc, argv, "2", &_filename, &_volume);
        Eng::Audio::Instance().me_play();
        break;
      }
      case 3: {
        rb_scan_args(argc, argv, "3", &_filename, &_volume, &_pitch);
        Eng::Audio::Instance().me_play();
        break;
      }
      default: {
        throw std::runtime_error("Audio.bgm_play takes 1 to 3 arguments, but were received " + std::to_string(argc));
      }
    }
    return Qnil;
  }

  static VALUE me_stop(VALUE self)
  {
    Eng::Audio::Instance().me_stop();
    return Qnil;
  }

  static VALUE me_fade(VALUE self, VALUE _time)
  {
    int time = Convert::toCInt(_time);

    Eng::Audio::Instance().me_fade(time);
    return Qnil;
  }

  /*
    SE
  */

  static VALUE se_play(int argc, VALUE *argv, VALUE self)
  {
    CStr fileName = Convert::toCStr(argv[0]);
    switch (argc) {
      case 1: {
        Eng::Audio::Instance().se_play(fileName);
        break;
      }
      case 2: {
        float volume = Convert::toCDouble2(argv[1]);
        Eng::Audio::Instance().se_play(fileName, volume);
        break;
      }
      case 3: {
        float volume = Convert::toCDouble2(argv[1]);
        float pitch = Convert::toCDouble2(argv[2]);
        Eng::Audio::Instance().se_play(fileName, volume, pitch / 100);
        break;
      }
      default: {
        throw std::runtime_error("Audio.bgm_play takes 1 to 3 arguments, but were received " + std::to_string(argc));
      }
    }
    return Qnil;
  }

  static VALUE se_stop(VALUE self)
  {
    Eng::Audio::Instance().se_stop();
    return Qnil;
  }

  static VALUE se_fade(VALUE self, VALUE _time)
  {
    int time = Convert::toCInt(_time);
    Eng::Audio::Instance().se_fade(time);
    return Qnil;
  }
};

}  // namespace It