#include "engnine/Bitmap.h"

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <cassert>
#include <cmath>
#include <stdexcept>

#include "AppDefs.h"
#include "Log.hpp"
#include "engnine/Color.hpp"
#include "engnine/FileUtils.hpp"
#include "engnine/Font.hpp"
#include "engnine/Rect.hpp"
#include "engnine/base/Fonts.h"
#include "engnine/internal/Texts.hpp"
#include "integrator/Convert.hpp"
#include "integrator/It_Bitmap.hpp"
#include "integrator/It_Font.hpp"

namespace Eng {

using app::CStr;

// Constructor

Bitmap::Bitmap(const char* assetName, VALUE rbObj) :
    EngineBase(rbObj),
    font(new Font()),
    renderTexture()
{
  sf::Image image;
  app::String filename = FileUtils::parseRtpPath(assetName);
  bool loaded = image.loadFromFile(filename);
  if (!loaded) {
    throw std::runtime_error("Could not load image.");
  }

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

  if (hasRbObj()) {
    bindRubyVars();
  }
}

// Constructor overload

Bitmap::Bitmap(unsigned int _width, unsigned int _height, VALUE rbObj) :
    EngineBase(rbObj),
    font(new Font()),
    renderTexture()
{
  width = _width;
  height = _height;

  sf::ContextSettings settings;
  settings.antialiasingLevel = 0;
  renderTexture.create(width, height, settings);
  renderTexture.clear(sf::Color::Transparent);

  isDisposed = false;
  dirty = false;

  if (hasRbObj()) {
    bindRubyVars();
  }
}

// Destructor

Bitmap::~Bitmap()
{
  // Log::out() << " - Bitmap (destructor)";
  dispose();
}

void Bitmap::initRubyObj()
{
  if (hasRbObj()) {
    return;
  }
  rbObj = It::Bitmap::createRubyObject(this);
  bindRubyVars();
}

// Bind props ruby object to instance object

void Bitmap::bindRubyVars()
{
  if (rbObj == Qnil) {
    throw std::runtime_error("Bitmap doesn't have rbObj defined.");
  }

  if (font->rbObj == Qnil) {
    font->rbObj = It::Font::createRubyObject(font);
  }

  rb_iv_set(rbObj, "@width", Convert::toRubyNumber(width));
  rb_iv_set(rbObj, "@height", Convert::toRubyNumber(height));

  rb_iv_set(rbObj, "@font", font->rbObj);
}

// Engine

const sf::Texture& Bitmap::getTexture()
{
  return renderTexture.getTexture();
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
  if (font == v) {
    return;
  }

  VALUE fontValue = Qnil;
  if (v) {
    if (!v->hasRbObj()) {
      font->rbObj = It::Font::createRubyObject(font);
    }
    fontValue = v->rbObj;
  }
  rb_iv_set(rbObj, "@font", fontValue);
  font = v;
}

/*
  Methods
*/

// Method dispose()

void Bitmap::dispose()
{
  // Log::out() << "dispose bitmap";
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
  sf::Color& sfColor = color->getSfColor();

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
  fill_rect(_rect->x.get(), _rect->y.get(), _rect->width.get(), _rect->height.get(), _color);
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
  sf::Image image = renderTexture.getTexture().copyToImage();
  image.setPixel(x, y, _color->getSfColor());
  texture.loadFromImage(image);

  sf::Sprite sprite;
  sprite.setTexture(texture);

  renderTexture.draw(sprite);
  renderTexture.display();

  dirty = true;
}

// Method hue_change

void Bitmap::hue_change(int _hue) { }

// Method draw_text

void Bitmap::draw_text(Rect rect, app::CStr str, TextAlign align)
{
  draw_text(rect.x.get(), rect.y.get(), static_cast<double>(rect.width.get()), static_cast<double>(rect.height.get()), str, align);
}

// Method draw_text

void Bitmap::draw_text(double x, double y, double width, double height, CStr str, TextAlign align)
{
  const sf::Font* fontPtr = Fonts::Instance().getFont("Arial-bold");

  if (!fontPtr) {
    Log::err() << "Requested font was not found.";
    return;
  }

  sf::Text text = Texts::createText(str);
  text.setFont(*fontPtr);
  text.setCharacterSize(font->getter_size());
  text.setLetterSpacing(1);
  text.setLineSpacing(1.5);

  const sf::Color& fillColor = font->getter_color()->getSfColor();
  text.setFillColor(fillColor);

  float lineSpacing = fontPtr->getLineSpacing(font->getter_size()) * 1.25;

  sf::Vector2f position;
  sf::Vector2f origin(0, lineSpacing / 2.0f);
  // text.setOrigin(0, bounds.top + bounds.height / 2.0f);

  sf::FloatRect bounds = text.getGlobalBounds();

  if (align == TextAlign::TEXT_LEFT) {
    position.x = x;
  } else if (align == TextAlign::TEXT_RIGHT) {
    position.x = std::floor(x + width);
    origin.x = std::round(bounds.width + bounds.left + 3.3);
  } else if (align == TextAlign::TEXT_CENTER) {
    origin.x = bounds.left;
    position.x = x + (width - bounds.width) / 2;
  }

  position.y = y + height / 2;
  text.setPosition(position);

  text.setOrigin(origin);

  sf::Vector2f scale(1, 1);
  if (bounds.width > width) {
    scale.x = width / bounds.width;
  }
  if (bounds.height > height) {
    scale.y = height / bounds.height;
  }

  scale.x -= .1;
  scale.y -= .12;

  text.setScale(scale);

  renderTexture.draw(text, sf::BlendAlpha);
  renderTexture.display();
}

// Method get_text_size

Eng::Rect* Bitmap::get_text_size(app::CStr str)
{
  const sf::Font* fontPtr = Fonts::Instance().getFont("Arial-bold");
  assert(fontPtr);

  sf::Text text;
  text.setFont(*fontPtr);
  text.setString(str);
  text.setCharacterSize(font->getter_size());
  text.setLetterSpacing(0.6);
  text.setLineSpacing(1.5);
  sf::FloatRect textBounds = text.getGlobalBounds();
  Eng::Rect* rect = new Eng::Rect(textBounds.left, textBounds.top, textBounds.width + 1, textBounds.height);
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

}  // namespace Eng