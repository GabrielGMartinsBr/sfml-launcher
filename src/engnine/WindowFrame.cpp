#include "engnine/WindowFrame.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cmath>

#include "engnine/Bitmap.h"
#include "engnine/Lists.hpp"

namespace Eng {

/*
  ⇩⇩⇩ Public ⇩⇩⇩
*/

WindowFrame::WindowFrame(const sf::View& view, Viewport* viewport) :
    view(view),
    viewport(viewport),
    backColor(sf::Color::White),
    borderColor(sf::Color::White)
{
  z = 0;
  width = 0;
  height = 0;
  opacity = 255;
  backOpacity = 255;
  visible = true;
  isDisposed = false;
}

void WindowFrame::onRender(sf::RenderTexture& renderTexture)
{
  renderTexture.setView(view);
  renderTexture.draw(backSprite);
  renderTexture.draw(borderSprite);
  renderTexture.display();
  renderTexture.setView(renderTexture.getDefaultView());
}

bool WindowFrame::shouldRender() const
{
  return !isDisposed && visible;
}

int WindowFrame::getZIndex() const
{
  return z;
}

void WindowFrame::setZ(int v)
{
  z = v;
  Lists::Instance().markZOrderDirty();
}

void WindowFrame::setOpacity(uint8_t value)
{
  opacity = value;
  borderColor.a = opacity;
  backColor.a = (opacity / 255.f) * (backOpacity / 255.f) * 255.f;
  borderSprite.setColor(borderColor);
  backSprite.setColor(backColor);
}

void WindowFrame::setBackOpacity(uint8_t value)
{
  backOpacity = value;
  borderColor.a = opacity;
  backColor.a = (opacity / 255.f) * (backOpacity / 255.f) * 255.f;
  borderSprite.setColor(borderColor);
  backSprite.setColor(backColor);
}

void WindowFrame::update(Bitmap* windowSkin)
{
  updateBackgroundSprite(windowSkin);
  updateBorder(windowSkin);
}

void WindowFrame::dispose()
{
  isDisposed = true;
  removeFromRenderList();
}

/*
  ⇩⇩⇩ Private ⇩⇩⇩
*/

void WindowFrame::updateBackgroundSprite(Bitmap* windowSkin)
{
  if (windowSkin == nullptr) {
    return;
  }

  float scaleX = (width - 2) / 128.0;
  float scaleY = (height - 2) / 128.0;

  backTexture = windowSkin->getTexture();
  backSprite.setTexture(backTexture);

  backSprite.setTextureRect(sf::IntRect(
    0, 0,
    128, 128
  ));

  backSprite.setScale(scaleX, scaleY);
  backSprite.setPosition(1, 1);
  backSprite.setColor(backColor);
}

void WindowFrame::updateBorder(Bitmap* windowSkin)
{
  if (windowSkin == nullptr) {
    return;
  }
  constexpr int BORDER_START_X = 128;
  constexpr int BORDER_START_Y = 0;
  constexpr int BORDER_END_X = 192;
  constexpr int BORDER_END_Y = 64;
  constexpr int CORNER_SIZE = 16;
  constexpr int BORDER_THICK = 16;
  constexpr int BORDER_SIZE = 64;
  constexpr int BORDER_LINE = BORDER_SIZE - CORNER_SIZE * 2;

  sf::Texture srcTexture(windowSkin->getTexture());

  sf::RenderTexture& rd = rendTex;
  rd.clear(sf::Color::Transparent);

  sf::IntRect topLeft(
    BORDER_START_X,
    BORDER_START_Y,
    CORNER_SIZE,
    CORNER_SIZE
  );
  sf::IntRect topRight(
    BORDER_END_X - CORNER_SIZE,
    BORDER_START_Y,
    CORNER_SIZE,
    CORNER_SIZE
  );
  sf::IntRect bottomRight(
    BORDER_END_X - CORNER_SIZE,
    BORDER_END_Y - CORNER_SIZE,
    CORNER_SIZE,
    CORNER_SIZE
  );
  sf::IntRect bottomLeft(
    BORDER_START_X,
    BORDER_END_Y - CORNER_SIZE,
    CORNER_SIZE,
    CORNER_SIZE
  );
  sf::IntRect top(
    BORDER_START_X + CORNER_SIZE,
    BORDER_START_Y,
    BORDER_LINE,
    BORDER_THICK
  );
  sf::IntRect bottom(
    BORDER_START_X + CORNER_SIZE,
    BORDER_END_Y - CORNER_SIZE,
    BORDER_LINE,
    BORDER_THICK
  );
  sf::IntRect left(
    BORDER_START_X,
    BORDER_START_Y + CORNER_SIZE,
    BORDER_THICK,
    BORDER_LINE
  );
  sf::IntRect right(
    BORDER_END_X - BORDER_THICK,
    BORDER_START_Y + CORNER_SIZE,
    BORDER_THICK,
    BORDER_LINE
  );

  sf::Sprite borderSpr;
  borderSpr.setTexture(srcTexture);

  // Top Left

  borderSpr.setTextureRect(topLeft);
  borderSpr.setPosition(0, 0);
  rd.draw(borderSpr);
  rd.display();

  // Top Right

  borderSpr.setTextureRect(topRight);
  borderSpr.setPosition(width - CORNER_SIZE, 0);
  rd.draw(borderSpr);
  rd.display();

  // Bottom Right

  borderSpr.setTextureRect(bottomRight);
  borderSpr.setPosition(width - CORNER_SIZE, height - CORNER_SIZE);
  rd.draw(borderSpr);
  rd.display();

  // Bottom Left

  borderSpr.setTextureRect(bottomLeft);
  borderSpr.setPosition(0, height - CORNER_SIZE);
  rd.draw(borderSpr);
  rd.display();

  // Top

  borderSpr.setTextureRect(top);
  int limitX = std::floor((width - CORNER_SIZE * 2) / BORDER_LINE);
  for (int i = 0; i < limitX; i++) {
    borderSpr.setPosition(CORNER_SIZE + BORDER_LINE * i, 0);
    rd.draw(borderSpr);
    rd.display();
  }

  int stopX = CORNER_SIZE + BORDER_LINE * limitX;
  int missingX = width - CORNER_SIZE - stopX;
  if (missingX > 0) {
    top.width = missingX;
    borderSpr.setTextureRect(top);
    borderSpr.setPosition(stopX, 0);
    rd.draw(borderSpr);
    rd.display();
  }

  // Bottom

  borderSpr.setTextureRect(bottom);
  int _y = height - CORNER_SIZE;
  for (int i = 0; i < limitX; i++) {
    borderSpr.setPosition(CORNER_SIZE + BORDER_LINE * i, _y);
    rd.draw(borderSpr);
    rd.display();
  }

  if (missingX > 0) {
    bottom.width = missingX;
    borderSpr.setTextureRect(bottom);
    borderSpr.setPosition(stopX, _y);
    rd.draw(borderSpr);
    rd.display();
  }

  // Left

  borderSpr.setTextureRect(left);
  int limitY = std::floor((height - CORNER_SIZE * 2) / BORDER_LINE);
  for (int i = 0; i < limitY; i++) {
    borderSpr.setPosition(0, CORNER_SIZE + BORDER_LINE * i);
    rd.draw(borderSpr);
    rd.display();
  }

  int stopY = CORNER_SIZE + BORDER_LINE * limitY;
  int missingY = height - CORNER_SIZE - stopY;
  if (missingY > 0) {
    left.height = missingY;
    borderSpr.setTextureRect(left);
    borderSpr.setPosition(0, stopY);
    rd.draw(borderSpr);
    rd.display();
  }

  // Right

  borderSpr.setTextureRect(right);
  int _x = width - BORDER_THICK;
  for (int i = 0; i < limitY; i++) {
    borderSpr.setPosition(_x, CORNER_SIZE + BORDER_LINE * i);
    rd.draw(borderSpr);
    rd.display();
  }

  if (missingY > 0) {
    right.height = missingY;
    borderSpr.setTextureRect(right);
    borderSpr.setPosition(_x, stopY);
    rd.draw(borderSpr);
    rd.display();
  }

  texture = rd.getTexture();
  borderSprite.setTexture(texture);
  borderSprite.setColor(borderColor);
  borderSprite.setPosition(0, 0);
}

}  // namespace Eng