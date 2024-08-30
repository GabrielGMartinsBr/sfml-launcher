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
}

Audio& Audio::Instance()
{
  assert(instance);
  return *instance;
}

void Audio::Destroy()
{
  assert(instance);
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

void Audio::bgm_play()
{
  //   Log::out() << "Audio bgm_play was called, but it is not implemented yet.";
}
void Audio::bgm_stop()
{
  //   Log::out() << "Audio bgm_stop was called, but it is not implemented yet.";
}
void Audio::bgm_fade(int time)
{
  //   Log::out() << "Audio bgm_fade was called, but it is not implemented yet.";
}
void Audio::bgs_play(String fileName, float volume, float pitch)
{
  fileName.append(".ogg");
  bgs.play(fileName, volume, pitch, true);
}
void Audio::bgs_stop()
{
  bgs.stop();
}
void Audio::bgs_fade(int time)
{
  bgs.startFade(timestamp, time / 1e3);
}

void Audio::me_play()
{
  //   Log::out() << "Audio me_play was called, but it is not implemented yet.";
}
void Audio::me_stop()
{
  //   Log::out() << "Audio me_stop was called, but it is not implemented yet.";
}
void Audio::me_fade(int time)
{
  //   Log::out() << "Audio me_fade was called, but it is not implemented yet.";
}
void Audio::se_play(String fileName, float volume, float pitch)
{
  fileName.append(".ogg");
  se.play(fileName, volume, pitch, false);
}
void Audio::se_stop()
{
  se.stop();
}
void Audio::se_fade(int time)
{
  se.startFade(timestamp, time / 1e3);
}

}  // namespace Eng