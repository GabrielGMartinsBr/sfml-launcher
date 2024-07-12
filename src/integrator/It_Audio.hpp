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
  }

 private:
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
};

}  // namespace It