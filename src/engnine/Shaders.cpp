#include "engnine/Shaders.h"

#include <cassert>
#include <memory>
#include <stdexcept>

#include "AppDefs.h"
#include "Log.hpp"
#include "engnine/FileUtils.hpp"

namespace Eng {

using app::FilePath;
using app::UPtr;
using std::make_unique;

static CStr shadersDir = "/run/media/home/common/gabrielmartins.dev/dev/cpp/orm-xp/sfml-launcher/shaders";

/*
  ⇩⇩⇩ Static ⇩⇩⇩
*/
static Shaders* instance = nullptr;

void Shaders::Init()
{
  assert(!instance);
  instance = new Shaders();
}

Shaders& Shaders::Instance()
{
  assert(instance);
  return *instance;
}

void Shaders::Destroy()
{
  assert(instance);
  delete instance;
  instance = nullptr;
}

/*
  ⇩⇩⇩ Instance ⇩⇩⇩
*/

Shaders::Shaders()
{
  fadeTransitionShader = loadFragment("FadeTransition.frag");
  imageTransitionShader = loadFragment("ImageTransition.frag");
}

UPtr<Shader> Shaders::loadFragment(CStr fileName)
{
  FilePath path = FileUtils::combine(shadersDir, fileName);

  UPtr<Shader> shader = make_unique<Shader>();

  if (!shader->loadFromFile(path.c_str(), Shader::Fragment)) {
    Log::err() << "Failed to load shader fragment from file: " << path;
    throw std::runtime_error("Load shader fragment error");
  }

  return shader;
}

}  // namespace Eng