#pragma once

#include <SFML/Graphics/Shader.hpp>

#include "AppDefs.h"

namespace Eng {

using app::CStr;
using app::UPtr;
using sf::Shader;

struct Shaders {
  /*
    ⇩⇩⇩ Static ⇩⇩⇩
  */

  static void Init();
  static Shaders& Instance();
  static void Destroy();

  /*
    ⇩⇩⇩ Instance ⇩⇩⇩
  */

  UPtr<Shader> fadeTransitionShader;
  UPtr<Shader> imageTransitionShader;
  UPtr<Shader> spriteFlash;
  UPtr<Shader> spriteColor;

  Shaders();

  UPtr<Shader> loadFragment(CStr fileName);
};

}  // namespace Eng