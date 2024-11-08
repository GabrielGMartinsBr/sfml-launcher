#include "engnine/MidiPlayer.h"

#include "Log.hpp"

namespace Eng {

/*
 *  MidiPlayer constructor
 */
MidiPlayer::MidiPlayer() :
    clock(EngineClock::Instance()),
    settings(nullptr),
    synth(nullptr),
    player(nullptr),
    aDriver(nullptr),
    initialized(false),
    fading(false),
    loopEnabled(false),
    endCbEmitted(false),
    volume(1.0f),
    initialVolume(0.0f),
    fadeStartTs(0),
    fadeDuration(0)
{
  setLogFunctions();
}

/*
 * Initialize MidiPlayer core components
 */
bool MidiPlayer::init()
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
bool MidiPlayer::play(CStr fileName, float volume_, float pitch)
{
  volume = volume_;
  fading = false;

  if (!initialized) {
    init();
  }

  if (isPlaying()) {
    fluid_player_stop(player);
    fluid_player_join(player);
    for (int i = 0; i < 16; i++) {
      fluid_synth_all_sounds_off(synth, i);
    }
  }

  delete_fluid_audio_driver(aDriver);
  delete_fluid_player(player);
  initPlayer();

  bool res;
  res = loadMidiFile(fileName);
  if (!res) {
    return false;
  }

  setVolume(volume);

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
void MidiPlayer::stop()
{
  stopPlayback();
}

/*
 * Restart playback from beginning
 */
void MidiPlayer::reset()
{
  setProgress(0);
  if (!isPlaying()) {
    startsPlayback();
  }
}

/*
 * Stop playback with fade effect
 */
void MidiPlayer::fade(int ts)
{
  fadeDuration = ts;
  fadeStartTs = clock.getTotalElapsedTime();
  initialVolume = volume;
  fading = true;
}

/*
 * Stop playback and release all FluidSynth resources
 */
void MidiPlayer::destroy()
{
  if (initialized) {
    fluid_player_stop(player);
    fluid_player_join(player);
    cleanup();
  }
}

/*--------------------------------------
 *    ⇩⇩⇩   Private Handlers    ⇩⇩⇩
 *--------------------------------------*/

/*
 * On player tick
 */
int MidiPlayer::onTick(void* data, int tick)
{
  MidiPlayer* inst = static_cast<MidiPlayer*>(data);
  inst->updateOnPlaybackEndEvent();
  inst->updateVolumeFade();

  return FLUID_OK;
}

/*
 * On playback end
 */
void MidiPlayer::onPlaybackEnd()
{
  endCbEmitted = true;

  stopPlayback();

  Log::out() << "onPlaybackEnd: " << loopEnabled;
  if (loopEnabled) {
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

void MidiPlayer::updateOnPlaybackEndEvent()
{
  if (isPlaybackFinished() && !endCbEmitted) {
    onPlaybackEnd();
  }
}

void MidiPlayer::updateVolumeFade()
{
  if (!fading || !isPlaying()) {
    return;
  }
  float elapsed = clock.getTotalElapsedTime() - fadeStartTs;
  if (elapsed >= fadeDuration) {
    // volume = 0;
    setVolume(0);
    fluid_player_stop(player);
    fluid_player_join(player);
    setVolume(initialVolume);
    fading = false;
  } else {
    float p = 1.0 - (elapsed / fadeDuration);
    setVolume(initialVolume * p);
  }
}

/*
 * Configure loggers behavior by log level
 */
void MidiPlayer::setLogFunctions()
{
  fluid_set_log_function(fluid_log_level::FLUID_WARN, nullptr, nullptr);
  fluid_set_log_function(fluid_log_level::FLUID_INFO, nullptr, nullptr);
  fluid_set_log_function(fluid_log_level::FLUID_DBG, nullptr, nullptr);
}

/*
 * Initialize settings object to FluidSynth instance
 */
void MidiPlayer::initSettings()
{
  settings = new_fluid_settings();
  fluid_settings_setint(settings, "audio.periods", 3);
  fluid_settings_setint(settings, "audio.period-size", 64);
  fluid_settings_setint(settings, "audio.realtime-prio", 0);
  fluid_settings_setint(settings, "player.reset-synth", 1);
}

/*
 * Initialize FluidSynth instance
 */
void MidiPlayer::initSynth()
{
  synth = new_fluid_synth(settings);
}

/*
 * Load SoundFont to FluidSynth instance
 */
bool MidiPlayer::loadSoundFont()
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
void MidiPlayer::initPlayer()
{
  player = new_fluid_player(synth);
  fluid_player_set_tick_callback(player, onTick, this);
  aDriver = new_fluid_audio_driver(settings, synth);
}

/*
 * Load MIDI file to player queue
 */
bool MidiPlayer::loadMidiFile(CStr fileName)
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
void MidiPlayer::setVolume(float gain)
{
  fluid_synth_set_gain(synth, gain);
}

/*
 * Set playback speed
 */
bool MidiPlayer::setSpeed(double tempo)
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
bool MidiPlayer::setProgress(float value)
{
  int totalTicks = fluid_player_get_total_ticks(player);
  int result = fluid_player_seek(player, value * totalTicks);
  Log::out() << "setProgress ticks: " << value * totalTicks;
  return result != FLUID_FAILED;
}

/*
 * Start playback
 */
bool MidiPlayer::startsPlayback()
{
  int result = fluid_player_play(player);
  if (result == FLUID_OK) {
    endCbEmitted = false;
    return true;
  }

  return false;
}

/*
 * Pause playback
 */
void MidiPlayer::pausePlayback()
{
}

/*
 * Stop playback
 */
bool MidiPlayer::stopPlayback()
{
  if (player && isPlaying()) {
    fluid_player_stop(player);
    return true;
  }
  return false;
}

/*
 * Cleanup
 */
void MidiPlayer::cleanup()
{
  initialized = false;
  if (player) {
    stopPlayback();
    fluid_player_join(player);
  }
  if (aDriver) {
    delete_fluid_audio_driver(aDriver);
    aDriver = nullptr;
  }
  if (player) {
    delete_fluid_player(player);
    player = nullptr;
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

}  // namespace Eng