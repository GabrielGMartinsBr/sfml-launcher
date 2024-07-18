#pragma once

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdlib>
#include <stdexcept>

#include "base/Log.hpp"
#include "engnine/Color.hpp"
#include "engnine/FileUtils.hpp"
#include "engnine/Font.hpp"
#include "engnine/Rect.hpp"
#include "engnine/internal/Texts.hpp"
#include "ruby.h"

namespace Eng {

enum TextAlign {
  TEXT_LEFT,
  TEXT_CENTER,
  TEXT_RIGHT
};

typedef const char* Str;

class Bitmap {
 public:
  VALUE ptr;

  bool _disposed = false;
  sf::RenderTexture renderTexture;
  // sf::Image image;

  unsigned int width;
  unsigned int height;

  bool dirty = false;

  Bitmap(const char* assetName) :
      font(nullptr),
      renderTexture()
  {
    std::string filename = FileUtils::parseRtpPath(assetName);
    sf::Image image;
    bool loaded = image.loadFromFile(filename);
    if (!loaded) {
      throw std::runtime_error("Could not load image.");
    }
    createFont();

    sf::Vector2u size = image.getSize();
    width = size.x;
    height = size.y;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    renderTexture.create(width, height, settings);
    renderTexture.clear(sf::Color::Transparent);

    sf::Sprite spr;
    sf::Texture texture;
    texture.loadFromImage(image);
    spr.setTexture(texture);
    renderTexture.draw(spr, sf::BlendAlpha);
    renderTexture.display();
  }

  Bitmap(unsigned int _width, unsigned int _height) :
      font(nullptr),
      renderTexture()
  {
    width = _width;
    height = _height;

    createFont();
    // image.create(width, height, sf::Color::Transparent);
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
    renderTexture.create(width, height, settings);
    renderTexture.setSmooth(false);
    renderTexture.clear(sf::Color::Transparent);
  }

  Font* getter_font()
  {
    Log::out() << font;
    return font;
  }

  void setter_font(Font* v)
  {
    Log::out() << v;
    font = v;
  }

  void dispose()
  {
    _disposed = true;
  }

  bool disposed()
  {
    return _disposed;
  }

  Rect get_rect() const
  {
    return Rect(0, 0, width, height);
  };

  void blt(){};

  void stretch_blt(){};

  void clear()
  {
  }

  Color* get_pixel(unsigned int _x, unsigned int _y)
  {
    sf::Texture texture = renderTexture.getTexture();
    sf::Image image = texture.copyToImage();
    sf::Color px = image.getPixel(_x, _y);
    Color* color = new Color(px.r, px.g, px.b);
    return color;
  }

  void set_pixel(unsigned int x, unsigned int y, Color* _color)
  {
    if (_disposed) {
      throw std::runtime_error("disposed bitmap");
    }
    if (x < 0 || x >= width || y < 0 || y >= height) {
      return;
    }
    sf::Texture texture = renderTexture.getTexture();
    sf::Image image = texture.copyToImage();
    sf::Color color(_color->red, _color->green, _color->blue);
    image.setPixel(x, y, color);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    renderTexture.draw(sprite);
    dirty = true;
  }

  void fill_rect(int x, int y, int width, int height, Color* color)
  {
    sf::Sprite sprite;
    sf::Image img;
    sf::Texture text;
    sf::Color sfColor;

    parseColor(sfColor, color);

    img.create(this->width, this->height, sf::Color::Transparent);

    int limitX = std::min<int>(x + width, this->width);
    int limitY = std::min<int>(y + height, this->height);

    for (int i = x; i < limitX; i++) {
      for (int j = y; j < limitY; j++) {
        img.setPixel(i, j, sfColor);
      }
    }

    text.loadFromImage(img);
    sprite.setTexture(text);
    renderTexture.draw(sprite, sf::BlendAlpha);
    renderTexture.display();

    dirty = true;
  };

  void fill_rect(Rect* _rect, Color* _color)
  {
    fill_rect(_rect->getter_x(), _rect->getter_y(), _rect->getter_width(), _rect->getter_height(), _color);
  };

  void hue_change(int _hue);

  void draw_text(int x, int y, int width, int height, Str str, TextAlign align = TextAlign::TEXT_LEFT)
  {
    Texts::drawText(renderTexture, x, y, *font, str);
    renderTexture.display();
  }

  void draw_text(Rect rect, Str str, TextAlign align = TextAlign::TEXT_LEFT)
  {
    Texts::drawText(renderTexture, rect.getter_x(), rect.getter_y(), *font, str);
    renderTexture.display();
  }

  int get_text_size(Str _str);


 private:
  Font* font;

  static void parseColor(sf::Color& dest, Color* src)
  {
    dest.r = src->red;
    dest.g = src->green;
    dest.b = src->blue;
    dest.a = src->alpha;
  }

  void createFont()
  {
    font = new Font();
    VALUE fontClass = rb_const_get(rb_cObject, rb_intern("Font"));
    font->ptr = Data_Wrap_Struct(fontClass, NULL, free, font);
  }
};

}  // namespace Eng