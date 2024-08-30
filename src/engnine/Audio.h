#pragma once

#include <SFML/Audio.hpp>

#include "AppDefs.h"
#include "engnine/base/AudioState.hpp"

namespace Eng {

using app::CStr;
using app::String;
using app::UInt;

class Audio {
 public:
  /*
    ⇩⇩⇩ Static ⇩⇩⇩
  */

  static void Init();
  static Audio& Instance();
  static void Destroy();

  /*
    ⇩⇩⇩ Instance ⇩⇩⇩
  */

  Audio();

  void update(UInt timestamp);

  // BGM
  void bgm_play();
  void bgm_stop();
  void bgm_fade(int time);

  // BGS
  void bgs_play(String fileName, float volume = 100, float pitch = 1.0f);
  void bgs_stop();
  void bgs_fade(int time);

  // ME
  void me_play();
  void me_stop();
  void me_fade(int time);

  // SE
  void se_play(String fileName, float volume = 100, float pitch = 1.0f);
  void se_stop();
  void se_fade(int time);

 private:
  /*
    ⇩⇩⇩ Private ⇩⇩⇩
  */

  UInt timestamp;
  AudioState bgs;
  AudioState se;
};
}  // namespace Eng