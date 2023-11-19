#pragma once

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <algorithm>

#include "base/NumberUtils.hpp"
#include "engnine/Bitmap.hpp"
#include "engnine/Color.hpp"
#include "engnine/RGSSViewport.hpp"
#include "engnine/Rect.hpp"
#include "engnine/Tone.hpp"
#include "engnine/Viewport.hpp"
#include "ruby.h"

const std::string vertexShader = R"(
    void main() {
        gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
        gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    }
)";

const std::string fragmentShader = R"(
    uniform sampler2D texture1;
    uniform sampler2D texture2;

    void main() {
        vec4 color1 = texture2D(texture1, gl_TexCoord[0].xy);
        vec4 color2 = texture2D(texture2, gl_TexCoord[0].xy);

        // vec4 result = vec4(abs(color2.rgb - color1.rgb), 255);
        vec4 result = vec4((vec3(1.0) - color2.rgb), 1.0);

        gl_FragColor = result;
    }
)";

namespace Eng {

class Sprite {
 public:
  sf::Sprite sprite;
  sf::Texture text;

  VALUE bitmap_ptr;  // TODO: Remove

  int zoom_x;
  int zoom_y;
  int angle;
  int mirror;
  int bush_depth;
  int blend_type;

  Sprite() :
      spriteColor(255, 255, 255, 255)
  {
  }

  Sprite(Viewport *_viewport) :
      Sprite()
  {
    viewport = _viewport;
  }

  /*
    Attr x
  */
  int getX() { return x; }
  void setX(int _x)
  {
    x = _x;
    dirty = true;
  }

  /*
    Attr y
  */
  int getY() { return y; }
  void setY(int _y)
  {
    y = _y;
    dirty = true;
  }

  /*
    Attr z
  */
  int getZ() { return z; }
  void setZ(int _z)
  {
    z = _z;
  }

  /*
    Attr ox
  */
  int getOx() { return ox; }
  void setOx(int _ox)
  {
    ox = _ox;
    dirty = true;
  }

  /*
    Attr oy
  */
  int getOy() { return oy; }
  void setOy(int _oy)
  {
    oy = _oy;
    dirty = true;
  }

  /*
    Attr opacity
  */
  unsigned int getOpacity() { return opacity; }
  void setOpacity(unsigned int v)
  {
    Num::clamp(v, 0u, 255u);
    opacity = v;
    spriteColor.a = v;
    dirty = true;
  }

  /*
    Attr bitmap
  */

  void setBitmap(Bitmap *_bitmap)
  {
    bitmap = _bitmap;
    dirty = true;
  }

  Bitmap *getBitmap()
  {
    return bitmap;
  }

  /*
   Attr src_rect
  */

  void setSrcRect(Rect *_src_rect)
  {
    src_rect = _src_rect;
    dirty = true;
  }

  Rect *getSrcRect()
  {
    return src_rect;
  }

  void setSrcRect(int x, int y, int w, int h)
  {
    sprite.setTextureRect(
      sf::IntRect(x, y, w, h)
    );
  }

  void dispose()
  {
    _disposed = true;
  }

  bool disposed()
  {
    return _disposed;
  }

  Viewport *getViewport()
  {
    return viewport;
  }

  void update() { }

  bool shouldRender()
  {
    return !_disposed && bitmap != nullptr && !bitmap->disposed();
  }

  void applyChanges()
  {
    if (!bitmap) {
      return;
    }

    if (bitmap->dirty) {
      // bitmap->buffer.
      text.loadFromImage(bitmap->buffer);
      sprite.setTexture(text);
      bitmap->dirty = false;
    }

    if (!dirty) {
      return;
    }

    sprite.setColor(spriteColor);

    if (src_rect) {
      setSrcRect(
        src_rect->x,
        src_rect->y,
        src_rect->width,
        src_rect->height
      );
    }

    sprite.setPosition(x - ox, y - oy);

    dirty = false;
  }

  void renderInViewport(RGSS::Viewport &defaultVp)
  {
    if (!shouldRender()) {
      return;
    }
    applyChanges();
    // defaultViewport.draw(
    //   sprite, sf::
    // );
    if (viewport) {
      // auto add = sf::BlendMode(
      //   sf::BlendMode::SrcColor,
      //   sf::BlendMode::DstColor,
      //   sf::BlendMode::Max
      // );
      //
      // defaultViewport.draw(
      //   sprite, blend
      // );
      // viewport->getRgssViewport().draw(
      //   sprite, sf::BlendMin
      // );
      sf::Shader shader;
      sf::Shader invertShader;
      if (!invertShader.loadFromFile("../shaders/invert_shader.frag", sf::Shader::Fragment)) {
        // Handle shader loading failure
      }

      defaultVp.renderTexture.display();

      // shader.setUniform("texture1", defaultViewport.renderTexture.getTexture());
      // shader.setUniform("texture2", sprite.getTexture());

      // shader.loadFromMemory(vertexShader, fragmentShader);

      // defaultViewport.draw(
      //   sprite, &shader
      // );

      viewport->getRgssViewport().draw(
        sprite, &invertShader
      );

      // viewport->getRgssViewport().draw(
      //   sprite, sf::BlendAlpha
      // );

      // defaultViewport.draw(
      //   sprite,
      //   sf::BlendMode(
      //     sf::BlendMode::One,
      //     sf::BlendMode::OneMinusSrcColor,
      //     sf::BlendMode::Equation::Subtract
      //   )

      // );

    } else {
      defaultVp.draw(
        sprite, sf::BlendNone
      );
    }
  }

 private:
  bool dirty = false;
  bool _disposed = false;

  Viewport *viewport = nullptr;
  Bitmap *bitmap = nullptr;
  Color *color = nullptr;
  Tone *tone = nullptr;
  Rect *src_rect = nullptr;

  float x = 0;
  float y = 0;
  int z = 0;
  bool visible = true;
  float ox = 0;
  float oy = 0;
  int opacity = 255;

  sf::Color spriteColor;
};

}  // namespace Eng