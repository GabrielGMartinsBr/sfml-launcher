#pragma once

#include <SFML/Graphics/Shader.hpp>
#include <stdexcept>

namespace Eng {

struct BlenShaders {
  sf::Shader vpNormalShader;
  sf::Shader sprNormalShader;
  sf::Shader sprInvertShader;

  void loadShaders()
  {
    if (
      !sprNormalShader.loadFromFile("../shaders/sprite_normal-blend.frag", sf::Shader::Fragment)
      || !sprInvertShader.loadFromFile("../shaders/sprite_invert-blend.frag", sf::Shader::Fragment)
      || !vpNormalShader.loadFromFile("../shaders/viewport_normal-blend.frag", sf::Shader::Fragment)
    ) {
      throw std::runtime_error("Load blend shaders failed.");
    }
  }
};

}  // namespace Eng