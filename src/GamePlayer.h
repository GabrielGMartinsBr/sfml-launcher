#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "GameInput.h"
#include "loadTexture.h"

struct GamePlayer {
  unsigned int x;
  unsigned int y;

  unsigned int next_x;
  unsigned int next_y;

  GamePlayer(unsigned int init_x, unsigned int init_y);

  void update();

  void draw(sf::RenderWindow &win);

 private:
  sf::Texture charsTexture;
  sf::IntRect spriteRect;
  sf::Sprite sprite;
};