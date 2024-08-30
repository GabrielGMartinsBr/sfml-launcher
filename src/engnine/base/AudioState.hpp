#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include "AppDefs.h"
#include "Log.hpp"
#include "engnine/FileUtils.hpp"

namespace Eng {

using app::String;
using app::UInt;
using sf::Sound;
using sf::SoundBuffer;

struct AudioState {
  AudioState()
  {
    volume = 100;
    fadeVolume = volume;
    isFading = false;
    fadeStartTs = 0;
    fadeDuration = 0;
  }

  void play(String& fileName, float _volume, float pitch, bool loop)
  {
    if (currentFileName != fileName) {
      loadSound(buffer, fileName.c_str());
      sound.setBuffer(buffer);
      currentFileName = fileName;
    }
    isFading = false;
    volume = _volume;
    sound.setVolume(volume);
    sound.setPitch(pitch);
    sound.play();
    sound.setLoop(loop);
  }

  void stop()
  {
    sound.stop();
  }

  void startFade(UInt ts, float duration)
  {
    isFading = true;
    fadeStartTs = ts;
    fadeDuration = duration;
  }

  void update(UInt ts)
  {
    if (!isFading) return;
    float progress = (ts - fadeStartTs) / fadeDuration;
    fadeVolume = volume * (1.0f - progress);
    sound.setVolume(fadeVolume);
    if (progress > 1) {
      sound.stop();
      isFading = false;
    }
  }

  static void loadSound(sf::SoundBuffer& buffer, const CStr fileName)
  {
    String filePath = FileUtils::resolveRtpPath(fileName);

    if (!buffer.loadFromFile(filePath)) {
      Log::err() << "Error: Could not load sound file!";
      return;
    }
  }

 private:
  float volume;
  UInt fadeStartTs;
  float fadeDuration;
  float fadeVolume;
  bool isFading;

  String currentFileName;
  Sound sound;
  SoundBuffer buffer;
};

}  // namespace Eng