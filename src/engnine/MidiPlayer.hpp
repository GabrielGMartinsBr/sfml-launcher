#pragma once

#include <fluidsynth.h>
#include <fluidsynth/midi.h>
#include <fluidsynth/sfont.h>

#include <cassert>

#include "AppDefs.h"
#include "Log.hpp"
#include "engnine/FileUtils.hpp"
#include "fluidsynth/log.h"
#include "fluidsynth/misc.h"
#include "fluidsynth/synth.h"

namespace Eng {

using app::CStr;
using app::FilePath;

class MidiPlayer {
 public:

  /*--------------------------------------
   *    ⇩⇩⇩   Public    ⇩⇩⇩
   *--------------------------------------*/

  /*
   *  MidiPlayer constructor
   */
  MidiPlayer() :
      settings(nullptr),
      synth(nullptr),
      player(nullptr),
      aDriver(nullptr),
      initialized(false)
  {
    setLogFunctions();
  }

  /*
   * Initialize MidiPlayer core components
   */
  bool init()
  {
    if (initialized) {
      return false;
    }
    initSettings();
    initSynth();
    loadSoundFont();
    initPlayer();
    initialized = true;
    return true;
  }

  /*
   * Load MIDI file to player and starts the playback
   */
  bool play(CStr fileName, float volume, float pitch)
  {
    if (!initialized) {
      init();
    }

    if (isPlaying()) {
      fluid_player_stop(player);
      fluid_player_join(player);
    }

    fluid_player_seek(player, fluid_player_get_total_ticks(player));
    setVolume(volume);

    bool res;
    res = loadMidiFile(fileName);
    if (!res) {
      return false;
    }

    res = setSpeed(pitch);
    if (!res) {
      Log::err() << "Failed to set player speed.";
      return false;
    }

    res = startsPlayback();
    if (!res) {
      Log::err() << "Starts playback failed.";
      return false;
    }

    return true;
  }

  /*
   * Stop playback
   */
  void stop()
  {
    stopPlayback();
  }

  /*
   * Restart playback from beginning
   */
  void reset()
  {
    setProgress(0);
    if (!isPlaying()) {
      startsPlayback();
    }
  }

  /*
   * Stop playback with fade effect
   */
  void fade(int ts) { }

  /*
   * Stop playback and release all FluidSynth resources
   */
  void destroy()
  {
    if (initialized) {
      fluid_player_stop(player);
      fluid_player_join(player);
      cleanup();
    }
  }

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

  fluid_settings_t* settings;
  fluid_synth_t* synth;
  fluid_player_t* player;
  fluid_audio_driver_t* aDriver;
  bool initialized;

  bool loop = true;

  bool endEmitted = false;

  /*--------------------------------------
   *    ⇩⇩⇩   Handlers    ⇩⇩⇩
   *--------------------------------------*/

  /*
   * On player tick
   */
  static int onTick(void* data, int tick)
  {
    MidiPlayer* inst = static_cast<MidiPlayer*>(data);
    if (inst->isPlaybackFinished() && !inst->endEmitted) {
      inst->onPlaybackEnd();
    }
    return FLUID_OK;
  }

  /*
   * On playback end
   */
  void onPlaybackEnd()
  {
    endEmitted = true;

    stopPlayback();

    Log::out() << "onPlaybackEnd: " << loop;
    if (loop) {
      bool result = setProgress(0.0);
      Log::out() << "setProgress: " << result;
      if (result) {
        startsPlayback();
      } else {
        Log::err() << "Loop restart failed.";
      }
    }
  }

  /*--------------------------------------
   *    ⇩⇩⇩   Private Methods   ⇩⇩⇩
   *--------------------------------------*/

  /*
   * Configure loggers behavior by log level
   */
  void setLogFunctions()
  {
    fluid_set_log_function(fluid_log_level::FLUID_WARN, nullptr, nullptr);
    fluid_set_log_function(fluid_log_level::FLUID_INFO, nullptr, nullptr);
    fluid_set_log_function(fluid_log_level::FLUID_DBG, nullptr, nullptr);
  }

  /*
   * Initialize settings object to FluidSynth instance
   */
  void initSettings()
  {
    settings = new_fluid_settings();
    fluid_settings_setint(settings, "audio.periods", 3);
    fluid_settings_setint(settings, "audio.period-size", 64);
    fluid_settings_setint(settings, "audio.realtime-prio", 0);
  }

  /*
   * Initialize FluidSynth instance
   */
  void initSynth()
  {
    synth = new_fluid_synth(settings);
  }

  /*
   * Load SoundFont to FluidSynth instance
   */
  bool loadSoundFont()
  {
    CStr filename = "/run/media/home/common/gabrielmartins.dev/dev/cpp/orm-xp/arl-sound/GMGSx.sf2";

    int sFid = fluid_synth_sfload(synth, filename, 1);
    if (sFid == FLUID_FAILED) {
      Log::err() << "Failed to load SoundFont: " << filename;
      cleanup();
      return false;
    }

    Log::err() << "SoundFont file was loaded successfully.";
    return true;
  }

  /*
   * Initialize MIDI player
   */
  void initPlayer()
  {
    aDriver = new_fluid_audio_driver(settings, synth);
    if (!player) {
      player = new_fluid_player(synth);
      fluid_player_set_tick_callback(player, onTick, this);
    }
  }

  /*
   * Load MIDI file to player queue
   */
  bool loadMidiFile(CStr fileName)
  {
    // Combine base dir with file name and extension
    FilePath filePath = FileUtils::getRtpPath();
    filePath.append(fileName);
    filePath.replace_extension(".mid");

    // Load file and add to queue
    int sFid = fluid_player_add(player, filePath.c_str());

    if (sFid == FLUID_FAILED) {
      cleanup();
      Log::err() << "Failed to load MIDI file: " << filePath.c_str();
      return false;
    }

    return true;
  }

  /*
   * Set playback Volume
   */
  void setVolume(float gain)
  {
    fluid_synth_set_gain(synth, gain);
  }

  /*
   * Set playback speed
   */
  bool setSpeed(double tempo)
  {
    int sFid = fluid_player_set_tempo(player, FLUID_PLAYER_TEMPO_INTERNAL, tempo);
    if (sFid == FLUID_FAILED) {
      cleanup();
      return false;
    }
    return true;
  }

  /*
   * Set playback position
   */
  bool setProgress(float value)
  {
    int totalTicks = fluid_player_get_total_ticks(player);
    int result = fluid_player_seek(player, value * totalTicks);
    Log::out() << "setProgress ticks: " << value * totalTicks;
    return result != FLUID_FAILED;
  }

  /*
   * Start playback
   */
  bool startsPlayback()
  {
    Log::out() << "player status: " << getPlayerStatusString();
    int result = fluid_player_play(player);
    Log::out() << "startsPlayback: " << result;
    if (result == FLUID_OK) {
      endEmitted = false;
      return true;
    }

    return false;
  }

  /*
   * Pause playback
   */
  void pausePlayback()
  {
  }

  /*
   * Stop playback
   */
  bool stopPlayback()
  {
    if (player && isPlaying()) {
      fluid_player_stop(player);
      return true;
    }
    return false;
  }

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

  /*
   * Cleanup
   */
  void cleanup()
  {
    initialized = false;
    if (player) {
      stopPlayback();
      fluid_player_join(player);
      delete_fluid_player(player);
      player = nullptr;
    }
    if (aDriver) {
      delete_fluid_audio_driver(aDriver);
      aDriver = nullptr;
    }
    if (synth) {
      delete_fluid_synth(synth);
      synth = nullptr;
    }
    if (settings) {
      delete_fluid_settings(settings);
      settings = nullptr;
    }
    delete_fluid_audio_driver(aDriver);
  }
};

}  // namespace Eng