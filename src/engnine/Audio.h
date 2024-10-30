#pragma once

#include <SFML/Audio.hpp>

#include "AppDefs.h"
#include "engnine/MidiPlayerWorker.hpp"
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
  void bgm_play(CStr fileName, float volume = 100, float pitch = 100);
  void bgm_stop();
  void bgm_fade(int time);
  void bgmReset();

  // BGS
  void bgs_play(String fileName, float volume = 100, float pitch = 100);
  void bgs_stop();
  void bgs_fade(int time);

  // ME
  void me_play(CStr fileName, float volume = 100, float pitch = 100);
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
  MidiPlayerWorker bgm;
  MidiPlayerWorker me;
  AudioState bgs;
  AudioState se;
};
}  // namespace Eng