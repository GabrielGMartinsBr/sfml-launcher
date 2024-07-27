#pragma once

#include "engnine/Bitmap.h"

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <stdexcept>

#include "engnine/Color.hpp"
#include "engnine/FileUtils.hpp"
#include "engnine/Font.hpp"
#include "engnine/Rect.hpp"
#include "engnine/internal/Texts.hpp"

namespace Eng {

Bitmap::Bitmap(const char* assetName) :
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

  isDisposed = false;
  dirty = false;
}

Bitmap::Bitmap(unsigned int _width, unsigned int _height) :
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

  isDisposed = false;
  dirty = false;
}

Bitmap::~Bitmap()
{
  // Log::out() << " - Bitmap (destructor)";
  dispose();
}

/*
  Properties
*/

// Getter font

Font* Bitmap::getter_font()
{
  return font;
}

// Setter font

void Bitmap::setter_font(Font* v)
{
  if (font != nullptr) {
    delete font;
    // font = nullptr;
  }
  // Log::out() << v;
  font = v;
}

/*
  Methods
*/

// Method dispose()

void Bitmap::dispose()
{
  isDisposed = true;
}

// Method disposed?()

bool Bitmap::disposed()
{
  return isDisposed;
}

// Method width

int Bitmap::getter_width()
{
  return width;
}

// Method height

int Bitmap::getter_height()
{
  return height;
}

// Method rect

Rect Bitmap::get_rect() const
{
  return Rect(0, 0, width, height);
};

// Method blt

void Bitmap::blt(int x, int y, Bitmap* src_bitmap, Rect* src_rect, int opacity)
{
  sf::Texture srcText = src_bitmap->renderTexture.getTexture();
  sf::Sprite spr(srcText, src_rect->sfRect());
  sf::Color color(255, 255, 255, opacity);
  spr.setPosition(x, y);
  spr.setColor(color);
  renderTexture.draw(spr, sf::BlendNone);
  renderTexture.display();
  dirty = true;
};

// Method stretch_blt

void Bitmap::stretch_blt(Rect* dst_rect, Bitmap* src_bitmap, Rect* src_rect, int opacity)
{
  sf::IntRect srcRect = src_rect->sfRect();
  sf::IntRect dstRect = dst_rect->sfRect();

  sf::Texture srcText = src_bitmap->renderTexture.getTexture();
  sf::Sprite spr(srcText, srcRect);
  sf::Color color(255, 255, 255, opacity);

  float scaleX = (float)dstRect.width / (float)srcRect.width;
  float scaleY = (float)dstRect.height / (float)srcRect.height;

  spr.setPosition(dstRect.left, dstRect.top);
  spr.setScale(scaleX, scaleY);
  spr.setColor(color);

  renderTexture.draw(spr, sf::BlendNone);
  renderTexture.display();
  dirty = true;
};

// Method fill_rect

void Bitmap::fill_rect(int x, int y, int width, int height, Color* color)
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

// Method fill_rect overloaded

void Bitmap::fill_rect(Rect* _rect, Color* _color)
{
  fill_rect(_rect->getter_x(), _rect->getter_y(), _rect->getter_width(), _rect->getter_height(), _color);
};

// Method clear

void Bitmap::clear()
{
  renderTexture.clear(sf::Color::Transparent);
  renderTexture.display();
  dirty = true;
}

// Method get_pixel

Color* Bitmap::get_pixel(unsigned int _x, unsigned int _y)
{
  sf::Texture texture = renderTexture.getTexture();
  sf::Image image = texture.copyToImage();
  sf::Color px = image.getPixel(_x, _y);
  Color* color = new Color(px.r, px.g, px.b);
  return color;
}

// Method set_pixel

void Bitmap::set_pixel(unsigned int x, unsigned int y, Color* _color)
{
  if (isDisposed) {
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

// Method hue_change

void Bitmap::hue_change(int _hue) { }

// Method draw_text

void Bitmap::draw_text(int x, int y, int width, int height, app::CStr str, TextAlign align)
{
  Texts::drawText(renderTexture, x, y, *font, str);
  renderTexture.display();
}

// Method draw_text

void Bitmap::draw_text(Rect rect, app::CStr str, TextAlign align)
{
  Texts::drawText(renderTexture, rect.getter_x(), rect.getter_y(), *font, str);
  renderTexture.display();
}

// Method get_text_size

Eng::Rect* Bitmap::get_text_size(app::CStr str)
{
  sf::Font sfFont;
  Texts::loadFont(sfFont);
  sf::Text text;
  text.setFont(sfFont);
  text.setString(str);
  text.setCharacterSize(24);
  sf::FloatRect textBounds = text.getLocalBounds();
  Eng::Rect* rect = new Eng::Rect(textBounds.left, textBounds.top, textBounds.width, textBounds.height);
  return rect;
}

// Static method parseColor

void Bitmap::parseColor(sf::Color& dest, Color* src)
{
  dest.r = src->red;
  dest.g = src->green;
  dest.b = src->blue;
  dest.a = src->alpha;
}

// Method createFont

void Bitmap::createFont()
{
  font = new Font();
}

}  // namespace Eng