#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/System/Vector2.hpp>
#include <stdexcept>

#include "base/Log.hpp"
#include "engnine/Color.hpp"
#include "engnine/FileUtils.hpp"
#include "engnine/Rect.hpp"

namespace Eng {

typedef const char* Str;

class Bitmap {
 public:
  bool _disposed = false;
  sf::Image buffer;

  unsigned int width;
  unsigned int height;

  bool dirty = false;

  Bitmap(const char* assetName)
  {
    std::string filename = FileUtils::parseRtpPath(assetName);
    bool loaded = buffer.loadFromFile(filename);
    if (!loaded) {
      throw std::runtime_error("Could not load image.");
    }

    sf::Vector2u size = buffer.getSize();
    width = size.x;
    height = size.y;
  }

  Bitmap(unsigned int _width, unsigned int _height) :
      buffer()
  {
    width = _width;
    height = _height;

    buffer.create(width, height, sf::Color::Transparent);
  }

  void dispose()
  {
    _disposed = true;
  }

  bool disposed()
  {
    return _disposed;
  }

  void get_rect(){};

  void blt(){};

  void stretch_blt(){};

  void clear();

  Color* get_pixel(unsigned int _x, unsigned int _y)
  {
    sf::Color px = buffer.getPixel(_x, _y);
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
    sf::Color color(_color->red, _color->green, _color->blue);
    buffer.setPixel(x, y, color);
    dirty = true;
  }

  void fill_rect(int _x, int _y, int _width, int _height, Color* _color)
  {
    sf::Color color;
    parseColor(color, _color);

    for (int x = _x; x < _width; x++) {
      for (int y = _x; y < _height; y++) {
        buffer.setPixel(x, y, color);
      }
    }

    dirty = true;
  };

  void fill_rect(Rect* _rect, Color* _color)
  {
    sf::Color color;
    parseColor(color, _color);

    for (int x = _rect->x; x < _rect->width; x++) {
      for (int y = _rect->y; y < _rect->height; y++) {
        buffer.setPixel(x, y, color);
      }
    }

    dirty = true;
  };

  void hue_change(int _hue);

  void draw_text(int _x, int _y, int _width, int _height, Str _str, int _align);

  void draw_text(Rect _rect, Str _str, int _align);

  int get_text_size(Str _str);

 private:
  static void parseColor(sf::Color& dest, Color* src)
  {
    dest.r = src->red;
    dest.g = src->green;
    dest.b = src->blue;
  }
};

}  // namespace Eng