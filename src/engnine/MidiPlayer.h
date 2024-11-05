#pragma once

#include <fluidsynth.h>
#include <fluidsynth/midi.h>
#include <fluidsynth/sfont.h>

#include <cassert>

#include "AppDefs.h"
#include "engine/EngineClock.h"
#include "engnine/FileUtils.hpp"

namespace Eng {

using app::CStr;
using app::FilePath;
using Eng::EngineClock;

class MidiPlayer {
 public:

  /*--------------------------------------
   *    ⇩⇩⇩   Public    ⇩⇩⇩
   *--------------------------------------*/

  MidiPlayer();

  bool init();
  bool play(CStr fileName, float volume_, float pitch);

  void startVolumeFadeOut(uint durationMs)
  {
  }

  void stop();
  void reset();
  void fade(int ts);

  void destroy();

  /*
   * Checks if the player is ready to start playback
   */
  inline bool isPlayerReady() const
  {
    return player && fluid_player_get_status(player) == FLUID_PLAYER_READY;
  }

  /*
   * Checks if the player is in the process of stopping
   */
  inline bool isStopping() const
  {
    return player && fluid_player_get_status(player) == FLUID_PLAYER_STOPPING;
  }

  /*
   * Checks if playback is currently active
   */
  inline bool isPlaying() const
  {
    return player && fluid_player_get_status(player) == FLUID_PLAYER_PLAYING;
  }

  /*
   * Checks if playback has finished all MIDI events
   */
  inline bool isPlayerDone() const
  {
    return player && fluid_player_get_status(player) == FLUID_PLAYER_DONE;
  }

  /*
   * Checks if playback has reached the end of the MIDI track
   */
  inline bool isPlaybackFinished() const
  {
    return fluid_player_get_current_tick(player) >= fluid_player_get_total_ticks(player);
  }

 private:
  /*--------------------------------------
   *    ⇩⇩⇩   Private    ⇩⇩⇩
   *--------------------------------------*/
  EngineClock& clock;

  fluid_settings_t* settings;
  fluid_synth_t* synth;
  fluid_player_t* player;
  fluid_audio_driver_t* aDriver;

  bool initialized;
  bool fading;
  bool loopEnabled;
  bool endCbEmitted;

  float volume;
  float initialVolume;
  uint fadeStartTs;
  uint fadeDuration;

  /*--------------------------------------
   *    ⇩⇩⇩   Handlers    ⇩⇩⇩
   *--------------------------------------*/

  static int onTick(void* data, int tick);
  void onPlaybackEnd();

  /*--------------------------------------
   *    ⇩⇩⇩   Private Methods   ⇩⇩⇩
   *--------------------------------------*/

  void updateOnPlaybackEndEvent();
  void updateVolumeFade();

  void setLogFunctions();

  void initSettings();
  void initSynth();
  bool loadSoundFont();
  void initPlayer();
  bool loadMidiFile(CStr fileName);

  void setVolume(float gain);
  bool setSpeed(double tempo);
  bool setProgress(float value);

  bool startsPlayback();
  void pausePlayback();
  bool stopPlayback();

  /*
   * Return player status name string
   */
  inline CStr getPlayerStatusString() const
  {
    const int status = fluid_player_get_status(player);
    switch (status) {
      case FLUID_PLAYER_READY:
        return "Ready";
      case FLUID_PLAYER_PLAYING:
        return "Playing";
      case FLUID_PLAYER_STOPPING:
        return "Stopping";
      case FLUID_PLAYER_DONE:
        return "Done";
      default:
        return "Unknow";
    }
  }

  // Cleanup
  void cleanup();
};

}  // namespace Eng