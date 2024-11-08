#include "Audio.h"

#include <cassert>

#include "AppDefs.h"
#include "engnine/FileUtils.hpp"

namespace Eng {

using app::String;

/*
  ⇩⇩⇩ Static ⇩⇩⇩
*/
static Audio* instance = nullptr;

void Audio::Init()
{
  assert(!instance);
  instance = new Audio();
  instance->bgm.startWorker();
  instance->me.startWorker();
}

Audio& Audio::Instance()
{
  assert(instance);
  return *instance;
}

void Audio::Destroy()
{
  assert(instance);

  instance->bgm.startWorker();
  instance->me.startWorker();

  delete instance;
  instance = nullptr;
}

/*
  ⇩⇩⇩ Instance ⇩⇩⇩
*/

Audio::Audio()
{
  timestamp = 0;
}

void Audio::update(UInt ts)
{
  timestamp = ts;

  bgs.update(ts);
}

/*
  ⇩⇩⇩   BGM  ⇩⇩⇩
*/

// Play
void Audio::bgm_play(CStr fileName, float volume, float pitch)
{
  bgm.play(fileName, volume / 100.0f, pitch / 100.0f);
}

// Stop
void Audio::bgm_stop()
{
  bgm.stop();
}

// Fade
void Audio::bgm_fade(int time)
{
  bgm.fade(time);
}

// Reset
void Audio::bgmReset()
{
  bgm.reset();
}

/*
  ⇩⇩⇩   BGS   ⇩⇩⇩
*/

// Play
void Audio::bgs_play(String fileName, float volume, float pitch)
{
  fileName.append(".ogg");
  bgs.play(fileName, volume, pitch, true);
}

// Stop
void Audio::bgs_stop()
{
  bgs.stop();
}

// Fade
void Audio::bgs_fade(int time)
{
  bgs.startFade(timestamp, time / 1e3);
}

/*
  ⇩⇩⇩   ME   ⇩⇩⇩
*/

// Play
void Audio::me_play(CStr fileName, float volume, float pitch)
{
  me.play(fileName, volume / 100.0f, pitch / 100.0f);
}

// Stop
void Audio::me_stop()
{
  me.stop();
}

// Fade
void Audio::me_fade(int time)
{
  me.fade(time);
}

// Reset
void Audio::meReset()
{
  me.reset();
}

/*
  ⇩⇩⇩   SE   ⇩⇩⇩
*/

// Play
void Audio::se_play(String fileName, float volume, float pitch)
{
  fileName.append(".ogg");
  se.play(fileName, volume, pitch, false);
}

// Stop
void Audio::se_stop()
{
  se.stop();
}

// Fade
void Audio::se_fade(int time)
{
  se.startFade(timestamp, time / 1e3);
}

}  // namespace Eng