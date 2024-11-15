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
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <stdexcept>

#include "AppDefs.h"
#include "Log.hpp"
#include "engnine/Color.hpp"
#include "engnine/Engine.h"
#include "engnine/FileUtils.hpp"
#include "engnine/Font.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Shaders.h"
#include "engnine/base/Fonts.h"
#include "engnine/base/Texts.hpp"
#include "integrator/Convert.hpp"
#include "integrator/It_Bitmap.hpp"
#include "integrator/It_Font.hpp"

namespace Eng {

using app::CStr;

// Constructor

Bitmap::Bitmap(const char* assetName, VALUE rbObj) :
    EngineBase(rbObj),
    renderTexture(),
    texture()
{
  sf::Image image;
  app::String filename = FileUtils::resolveRtpImagePath(assetName);
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

  texture.loadFromImage(image);
  sf::Sprite spr(texture);
  renderTexture.draw(spr, sf::BlendAdd);
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
    renderTexture()
{
  width = _width;
  height = _height;

  sf::ContextSettings settings;
  // settings.antialiasingLevel = 0;
  renderTexture.create(width, height, settings);
  renderTexture.clear(sf::Color::Transparent);

  isDisposed = false;
  dirty = false;

  if (hasRbObj()) {
    bindRubyVars();
  }
}

// Clone constructor

Bitmap::Bitmap(const Bitmap* bitmap) :
    EngineBase(Qnil),
    renderTexture()
{
  width = bitmap->width;
  height = bitmap->height;

  isDisposed = false;
  dirty = false;

  sf::ContextSettings settings;
  settings.antialiasingLevel = 0;
  renderTexture.create(width, height, settings);
  renderTexture.clear(sf::Color::Transparent);

  sf::Sprite contents(bitmap->getTexture());
  renderTexture.draw(contents, sf::BlendAdd);
  renderTexture.display();
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
  VALUE rbId = It::Bitmap::createRubyObject(this);
  setRbId(rbId);
  bindRubyVars();
}

// Bind props ruby object to instance object

void Bitmap::bindRubyVars()
{
  if (rbObj == Qnil) {
    throw std::runtime_error("Bitmap doesn't have rbObj defined.");
  }
  rb_iv_set(rbObj, "@width", Convert::toRubyNumber(width));
  rb_iv_set(rbObj, "@height", Convert::toRubyNumber(height));
  if (!font) {
    rb_iv_set(rbObj, "@font", Qnil);
  } else if (font->rbObj == Qnil) {
    font->setRbId(It::Font::createRubyObject(font));
    rb_iv_set(rbObj, "@font", font->rbObj);
  }
}

// Engine

const sf::Texture& Bitmap::getTexture() const
{
  // renderTexture.display();
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

void Bitmap::setter_font(Font* value)
{
  if (font == value) {
    return;
  }

  font = value;
  if (!font->hasRbObj()) {
    font->rbObj = It::Font::createRubyObject(value);
  }
  rb_iv_set(rbObj, "@font", font->rbObj);
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
  renderTexture.draw(spr, sf::BlendAlpha);
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

  renderTexture.draw(spr, sf::BlendAlpha);
  renderTexture.display();
  dirty = true;
};

// Method fill_rect

void Bitmap::fill_rect(int x, int y, int width, int height, Color* color)
{
  sf::Sprite sprite;
  sf::Image img;
  sf::Texture text;
  const sf::Color& sfColor = color->getSfColor();

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
  Color* color = new Color(px.r, px.g, px.b, 255);
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

  renderTexture.draw(sprite, sf::BlendAlpha);
  renderTexture.display();

  dirty = true;
}

// Method hue_change

void Bitmap::hue_change(int hue)
{
  texture.update(renderTexture.getTexture());
  sprite.setTexture(texture);
  renderTexture.clear(sf::Color::Transparent);
  Shaders::Instance().changeHue->setUniform("hue", hue / 360.f);
  renderTexture.draw(sprite, Shaders::Instance().changeHue.get());
  renderTexture.display();
}

// Method draw_text

void Bitmap::draw_text(Rect rect, app::CStr str, TextAlign align)
{
  draw_text(rect.x.get(), rect.y.get(), static_cast<double>(rect.width.get()), static_cast<double>(rect.height.get()), str, align);
}

// Method draw_text

void Bitmap::draw_text(double x, double y, double width, double height, CStr str, TextAlign align)
{
  auto fontNames = font->getter_name();
  app::String fontName = fontNames.size() > 0 ? fontNames.at(0) : "Arial";
  const sf::Font* fontPtr = Fonts::Instance().getFont(fontName);

  if (!fontPtr) {
    Log::err() << "Requested font was not found.";
    Log::err() << "FontName: " << fontName;
    fontPtr = Fonts::Instance().getFont("Roboto");
    if (!fontPtr) return;
  }

  sf::Text text = Texts::createText(str);
  text.setFont(*fontPtr);
  text.setCharacterSize(font->getter_size());

  const sf::Color& fillColor = font->getter_color()->getSfColor();
  text.setFillColor(fillColor);

  float lineSpacing = fontPtr->getLineSpacing(font->getter_size()) * 1.25;

  sf::Vector2f position;
  sf::Vector2f origin(0, lineSpacing / 2.0f);

  sf::FloatRect bounds = text.getGlobalBounds();

  sf::Vector2f scale(1, 1);
  if (bounds.width > width) {
    scale.x = width / bounds.width;
  }
  if (bounds.height > height) {
    scale.y = height / bounds.height;
  }

  text.setScale(scale);

  if (align == TextAlign::TEXT_LEFT) {
    position.x = x;
  } else if (align == TextAlign::TEXT_RIGHT) {
    position.x = std::floor(x + width);
    origin.x = std::round(bounds.width * scale.x + bounds.left * scale.x);
  } else if (align == TextAlign::TEXT_CENTER) {
    origin.x = bounds.left * scale.x;
    position.x = x + (width - bounds.width * scale.x) / 2;
  }

  position.y = y + height / 2;
  text.setPosition(position);
  text.setOrigin(origin);

  sf::FloatRect localBounds = text.getLocalBounds();

  sf::RectangleShape shape(sf::Vector2f(width, height));
  shape.setFillColor(sf::Color::Blue);

  renderTexture.draw(text, sf::BlendNone);
  renderTexture.display();

  dirty = true;
}

// Method get_text_size

Eng::Rect* Bitmap::get_text_size(app::CStr str)
{
  auto fontNames = font->getter_name();
  app::String fontName = fontNames.size() > 0 ? fontNames.at(0) : "Arial";
  const sf::Font* fontPtr = Fonts::Instance().getFont(fontName);
  assert(fontPtr);

  sf::Text text;
  text.setFont(*fontPtr);
  text.setString(str);
  text.setCharacterSize(font->getter_size());
  sf::FloatRect textBounds = text.getGlobalBounds();
  Eng::Rect* rect = new Eng::Rect(textBounds.left, textBounds.top, textBounds.width + 1, textBounds.height);
  return rect;
}

}  // namespace Eng