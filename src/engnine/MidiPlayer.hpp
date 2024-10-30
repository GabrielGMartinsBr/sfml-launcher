#pragma once

#include <fluidsynth.h>
#include <fluidsynth/midi.h>
#include <fluidsynth/sfont.h>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <cmath>

#include "Log.hpp"
#include "fluidsynth/log.h"
#include "fluidsynth/synth.h"

namespace Eng {

struct MidiPlayer {
  const char* fontFile = "/run/media/home/common/gabrielmartins.dev/dev/cpp/orm-xp/arl-sound/GMGSx.sf2";
  const char* midiFile = "/run/media/home/common/gabrielmartins.dev/dev/cpp/orm-xp/sfml-launcher/GameData/Standard/Audio/BGM/064-Slow07.mid";

  void test()
  {
    fluid_set_log_function(fluid_log_level::FLUID_WARN, nullptr, nullptr);
    fluid_set_log_function(fluid_log_level::FLUID_INFO, nullptr, nullptr);
    fluid_set_log_function(fluid_log_level::FLUID_DBG, nullptr, nullptr);

    settings = new_fluid_settings();
    synth = new_fluid_synth(settings);

    // Load a SoundFont
    int sFid = fluid_synth_sfload(synth, fontFile, 1);
    if (sFid == FLUID_FAILED) {
      Log::err() << "Failed to load SoundFont\n";
      cleanUp();
      return;
    }

    aDriver = new_fluid_audio_driver(settings, synth);

    // Create a MIDI player
    player = new_fluid_player(synth);

    if (fluid_player_add(player, midiFile) != FLUID_OK) {
      Log::err() << "Failed to load MIDI file: " << midiFile;
      cleanUp();
      return;
    }

    // fluid_player_set_tempo(player, FLUID_PLAYER_TEMPO_INTERNAL, 1.5);
    // fluid_synth_set_gain(synth, 0.01f);
    // fluid_synth_set_

    // for (int channel = 0; channel < 16; ++channel) {
    //   fluid_synth_cc(synth, channel, 7, 1);  // Set each channel to 100 volume
    // }

    fluid_player_play(player);

    // fluid_synth_set_gain(synth, 0.5f);
    fluid_synth_set_gain(synth, 1.0f);
    fluid_player_set_tempo(player, FLUID_PLAYER_TEMPO_INTERNAL, 1.0);
  }

  void dispose()
  {
    cleanUp();
  }

 private:
  fluid_settings_t* settings;
  fluid_synth_t* synth;
  fluid_player_t* player;
  fluid_audio_driver_t* aDriver;

  void cleanUp()
  {
    if (player) {
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
  }
};

}  // namespace Eng