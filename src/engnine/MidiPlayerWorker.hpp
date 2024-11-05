#pragma once

#include "AppDefs.h"
#include "engnine/MidiPlayer.h"

namespace Eng {

using app::ConditionVariable;
using app::CStr;
using app::Mutex;
using app::Queue;
using app::Thread;

class MidiPlayerWorker {
 public:

  MidiPlayerWorker() :
      running(false) { }

  void startWorker()
  {
    running = true;
    playerThread = Thread(&MidiPlayerWorker::run, this);
  }

  void stopWorker()
  {
    if (!running) {
      return;
    }
    {
      std::lock_guard<std::mutex> lock(queueMutex);
      running = false;
    }
    player.stop();
    condition.notify_all();
    if (playerThread.joinable()) {
      playerThread.join();
    }
  }

  void enqueueAsyncTask(const std::function<void(MidiPlayer &)> &task)
  {
    {
      std::lock_guard<std::mutex> lock(queueMutex);
      tasks.push([task, this] { task(player); });  // Capture socket and task
    }
    condition.notify_one();  // Notify the worker thread
  }

  /*
    ⇩⇩⇩ Tasks ⇩⇩⇩
  */

  void play(CStr fileName, float volume, float pitch)
  {
    enqueueAsyncTask([fileName, volume, pitch](MidiPlayer &player) {
      player.play(fileName, volume, pitch);
    });
  }

  void stop()
  {
    enqueueAsyncTask([](MidiPlayer &player) {
      player.stop();
    });
  }

  void fade(int time)
  {
    enqueueAsyncTask([time](MidiPlayer &player) {
      player.fade(time);
    });
  }

  void reset()
  {
    enqueueAsyncTask([](MidiPlayer &player) {
      player.reset();
    });
  }

 private:
  bool running;
  Mutex queueMutex;
  ConditionVariable condition;
  Queue<std::function<void()>> tasks;
  Thread playerThread;
  MidiPlayer player;

  void run()
  {
    player.init();

    while (running) {
      std::function<void()> task;
      {
        std::unique_lock<std::mutex> lock(queueMutex);
        condition.wait(lock, [this] { return !tasks.empty() || !running; });

        if (!running) break;
        if (tasks.empty()) continue;

        task = std::move(tasks.front());
        tasks.pop();
      }

      if (task) {
        task();
      }
    }

    player.destroy();
  }
};

}  // namespace Eng