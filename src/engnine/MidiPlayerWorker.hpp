#pragma once

#include "AppDefs.h"
#include "Log.hpp"
#include "engnine/MidiPlayer.hpp"

namespace Eng {

using app::Atomic;
using app::CStr;
using app::Thread;

class MidiPlayerWorker {
 public:

  MidiPlayerWorker()
  {
    running.store(false);
  }

  void play(CStr fileName, float volume = 1.0f, float pitch = 1.0f)
  {
    stop();
    if (playerThread.joinable()) {
      playerThread.join();
    }
    playerThread = std::thread(&MidiPlayerWorker::run, this, fileName, volume, pitch);
  }

  void stop()
  {
    player.stop();
    running.store(false);
  }

  void fade(int time) { }

  void dispose()
  {
    if (!running.load()) {
      return;
    }
    player.destroy();
    running.store(false);
    if (playerThread.joinable()) {
      playerThread.join();
    }
  }

 private:
  Thread playerThread;
  MidiPlayer player;
  Atomic<bool> running;

  void run(CStr fileName, float volume, float pitch)
  {
    Log::out() << "fileName: " << fileName;
    Log::out() << "volume: " << volume;
    Log::out() << "pitch: " << pitch;

    player.play(fileName, volume, pitch);

    running.store(true);


    while (running.load()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    player.destroy();

    Log::out() << "Playback finished.";
  }
};

}  // namespace Eng