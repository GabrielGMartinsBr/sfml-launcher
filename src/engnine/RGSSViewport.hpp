#pragma once

/*
 * The RGSSViewport class provides functionality
 * to simulate/replicate the behavior of an
 * RGSS1 viewport within the SFML framework.
 */

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace RGSS {

using namespace sf;

class Viewport {
  RenderTexture renderTexture;
  Sprite sprite;

  int x;
  int y;
  int width;
  int height;

 public:
  Viewport(
    float _x,
    float _y,
    unsigned int _width,
    unsigned int _height
  ) :
      x(_x),
      y(_y),
      width(_width),
      height(_height)
  {
    sprite.setPosition(x, y);
    renderTexture.create(width, height);
    renderTexture.clear(Color::Transparent);
  }

  void draw(const Drawable& drawable, const RenderStates& states = RenderStates::Default)
  {
    renderTexture.draw(drawable, states);
  }

  void renderIn(RenderWindow& window)
  {
    sprite.setTexture(renderTexture.getTexture(), true);
    window.draw(sprite);
  }

  void setPosition(float x, float y)
  {
    sprite.setPosition(x, y);
  }

  void setPosition(const Vector2f& position)
  {
    sprite.setPosition(position);
  }

  void setSize(unsigned int _width, unsigned int _height)
  {
    // Try to release previous resources
    renderTexture.clear();

    width = _width;
    height = _height;
    renderTexture.create(width, height);
    renderTexture.clear(Color::Transparent);
  }

  void display()
  {
    renderTexture.display();
  }
};

}  // namespace RGSS