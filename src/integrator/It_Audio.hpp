#pragma once

#include <stdexcept>
#include <string>

#include "engnine/Audio.hpp"
#include "ruby.h"

namespace It {

class Audio {
 public:
  static void integrate()
  {
    VALUE audioModule = rb_define_class("Audio", rb_cObject);

    rb_define_module_function(audioModule, "bgm_play", RUBY_METHOD_FUNC(bgm_play), -1);
    rb_define_module_function(audioModule, "bgm_stop", RUBY_METHOD_FUNC(bgm_stop), 0);

    rb_define_module_function(audioModule, "bgs_play", RUBY_METHOD_FUNC(bgs_play), -1);
    rb_define_module_function(audioModule, "bgs_stop", RUBY_METHOD_FUNC(bgs_stop), 0);

    rb_define_module_function(audioModule, "me_play", RUBY_METHOD_FUNC(me_play), -1);
    rb_define_module_function(audioModule, "me_stop", RUBY_METHOD_FUNC(me_stop), 0);

    rb_define_module_function(audioModule, "se_play", RUBY_METHOD_FUNC(se_play), -1);
    rb_define_module_function(audioModule, "se_stop", RUBY_METHOD_FUNC(se_stop), 0);
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
        Eng::Audio::bgm_play();
        break;
      }
      case 2: {
        rb_scan_args(argc, argv, "2", &_filename, &_volume);
        Eng::Audio::bgm_play();
        break;
      }
      case 3: {
        rb_scan_args(argc, argv, "3", &_filename, &_volume, &_pitch);
        Eng::Audio::bgm_play();
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
    Eng::Audio::bgm_stop();
    return Qnil;
  }

  /*
    BGS
  */

  static VALUE bgs_play(int argc, VALUE *argv, VALUE self)
  {
    VALUE _filename, _volume, _pitch;
    switch (argc) {
      case 1: {
        rb_scan_args(argc, argv, "1", &_filename);
        Eng::Audio::bgs_play();
        break;
      }
      case 2: {
        rb_scan_args(argc, argv, "2", &_filename, &_volume);
        Eng::Audio::bgs_play();
        break;
      }
      case 3: {
        rb_scan_args(argc, argv, "3", &_filename, &_volume, &_pitch);
        Eng::Audio::bgs_play();
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
    Eng::Audio::bgs_stop();
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
        Eng::Audio::me_play();
        break;
      }
      case 2: {
        rb_scan_args(argc, argv, "2", &_filename, &_volume);
        Eng::Audio::me_play();
        break;
      }
      case 3: {
        rb_scan_args(argc, argv, "3", &_filename, &_volume, &_pitch);
        Eng::Audio::me_play();
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
    Eng::Audio::me_stop();
    return Qnil;
  }

  /*
    SE
  */

  static VALUE se_play(int argc, VALUE *argv, VALUE self)
  {
    VALUE _filename, _volume, _pitch;
    switch (argc) {
      case 1: {
        rb_scan_args(argc, argv, "1", &_filename);
        Eng::Audio::se_play();
        break;
      }
      case 2: {
        rb_scan_args(argc, argv, "2", &_filename, &_volume);
        Eng::Audio::se_play();
        break;
      }
      case 3: {
        rb_scan_args(argc, argv, "3", &_filename, &_volume, &_pitch);
        Eng::Audio::se_play();
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
    Eng::Audio::se_stop();
    return Qnil;
  }
};

}  // namespace It