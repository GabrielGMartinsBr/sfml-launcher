#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "base/Log.hpp"
#include "engnine/Bitmap.hpp"
#include "engnine/Color.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Tone.hpp"
#include "ruby.h"

namespace Eng {

class Sprite {

 public:
  sf::Sprite sprite;
  sf::Texture text;

  sf::Image buff;

  Bitmap *bitmap = nullptr;
  VALUE bitmap_ptr;
  Rect src_rect;
  bool visible;
  int x;
  int y;
  int z;
  int ox;
  int oy;
  int zoom_x;
  int zoom_y;
  int angle;
  int mirror;
  int bush_depth;
  int opacity;
  int blend_type;
  Color color;
  Tone tone;

  Sprite() :
      color(0, 0, 0),
      tone(0, 0, 0)
  {
    // atualizar();

    // buff.create(300, 300, sf::Color::Black);
    // text.loadFromImage(buff);
    // sprite.setTexture(text);

    // buff.setPixel(100, 100, sf::Color::White);
    // buff.setPixel(101, 100, sf::Color::White);
    // buff.setPixel(102, 100, sf::Color::White);
    // buff.setPixel(103, 100, sf::Color::White);
    // text.update(buff);
  }

  void atualizar()
  {
    if (bitmap) {
      //   // sf::IntRect rect(0, 0, 500, 300);
      //   // text.update(bitmap->buffer);

      //   bitmap->buffer.setPixel(0, 50, sf::Color::Red);
      // buff.setPixel(102, 100, sf::Color::White);

      text.loadFromImage(bitmap->buffer);
      sprite.setTexture(text);

      // bitmap->buffer.setPixel(100, 100, sf::Color::White);
      // bitmap->buffer.setPixel(101, 100, sf::Color::White);
      // bitmap->buffer.setPixel(102, 100, sf::Color::White);
      // bitmap->buffer.setPixel(103, 100, sf::Color::White);
      // text.update(buff);

      // Log::out() << bitmap->buffer.getSize().x;

      //   // sprite.setPosition(0, 0);
      //   // sprite.setTexture(text);
    }
  }

  void dispose() { }

  void disposed() { }

  void viewport() { }

  void update()
  {
    atualizar();
  }
};

}  // namespace Eng