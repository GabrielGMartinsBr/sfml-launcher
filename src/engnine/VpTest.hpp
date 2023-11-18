#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf {

class VpTest {
  unsigned int wid = 320;
  unsigned int hei = 240;

  RenderTexture vp_texture;
  Sprite vp_spr;
  Vector2f vp_position;

  Rect<int> rect;
  Sprite rect_spr;
  Texture rect_tex;
  Image rect_bitmap;

 public:
  VpTest() :
      vp_position(128, 64),
      rect(0, 0, 640 - 64, 480 - 64)
  {
    vp_spr.setPosition(vp_position);
    vp_texture.create(wid, hei);
    vp_texture.clear(sf::Color::Transparent);

    createGradientRect();

    vp_texture.draw(rect_spr);
    vp_texture.display();

    // vp_texture.draw(rect_spr);
  }

  void draw(RenderWindow& window)
  {
    // vp_spr.setTexture(vp_texture.getTexture());
    vp_spr.setTexture(vp_texture.getTexture(), true);
    window.draw(vp_spr);

    // window.draw(rect_spr);
  }

 private:

  void createGradientRect()
  {
    rect_bitmap.create(rect.width, rect.height, sf::Color::Transparent);

    float px = 0;
    float py = 0;
    int red = 0;
    int green = 0;

    for (int x = 0; x < rect.width; x++) {
      for (int y = 0; y < rect.height; y++) {
        px = static_cast<float>(x) / rect.width;
        py = static_cast<float>(y) / rect.width;
        red = px * 255;
        green = py * 255;
        rect_bitmap.setPixel(x, y, Color(red, green, 255));
      }
    }

    rect_tex.loadFromImage(rect_bitmap);
    rect_spr.setTexture(rect_tex);

    rect_spr.setPosition(0, 0);
    // rect_spr.setPosition(rect.left, rect.top);
  }
};

}  // namespace sf
