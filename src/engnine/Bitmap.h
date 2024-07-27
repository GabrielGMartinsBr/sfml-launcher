#pragma once

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

#include "AppDefs.h"
#include "engnine/Color.hpp"
#include "engnine/EngineBase.hpp"
#include "engnine/Font.hpp"
#include "engnine/Rect.hpp"

namespace Eng {

enum TextAlign {
  TEXT_LEFT,
  TEXT_CENTER,
  TEXT_RIGHT
};

class Bitmap : public EngineBase {
 public:
  sf::RenderTexture renderTexture;
  bool dirty = false;

  Bitmap(const char* assetName);
  Bitmap(unsigned int _width, unsigned int _height);
  ~Bitmap();

  // Properties

  Font* getter_font();
  void setter_font(Font* v);

  // Methods

  void dispose();
  bool disposed();

  int getter_width();
  int getter_height();

  Rect get_rect() const;

  void blt(int x, int y, Bitmap* src_bitmap, Rect* src_rect, int opacity = 255);
  void stretch_blt(Rect* dst_rect, Bitmap* src_bitmap, Rect* src_rect, int opacity = 255);

  void fill_rect(int x, int y, int width, int height, Color* color);
  void fill_rect(Rect* _rect, Color* _color);

  void clear();

  Color* get_pixel(unsigned int _x, unsigned int _y);
  void set_pixel(unsigned int x, unsigned int y, Color* _color);

  void hue_change(int _hue);

  void draw_text(int x, int y, int width, int height, app::CStr str, TextAlign align = TextAlign::TEXT_LEFT);
  void draw_text(Rect rect, app::CStr str, TextAlign align = TextAlign::TEXT_LEFT);
  Eng::Rect* get_text_size(app::CStr str);

 private:

  Font* font;
  unsigned int width;
  unsigned int height;
  bool isDisposed;

  static void parseColor(sf::Color& dest, Color* src);
  void createFont();
};

}  // namespace Eng