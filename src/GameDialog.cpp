#include "GameDialog.h"

#include "GameFonts.h"
#include "GameInput.h"

extern GameInput game_input;

namespace gc {
static const sf::Color Blue = sf::Color(0x3f, 0x7f, 0xaf);
static const sf::Color DarkGray = sf::Color(0x33, 0x33, 0x33);
static const sf::Color LightGray = sf::Color(0xcf, 0xcf, 0xcf);
};  // namespace gc

static const auto backColor = gc::DarkGray;

GameDialog::GameDialog(
  const sf::String& title,
  float x, float y, float width, float height
) :
    border_width(3),
    frame_height(24),
    position(x, y),
    size(width, height),
    client_position(x, y),
    client_size(width - border_width * 2, height - border_width - frame_height),
    backShape(size),
    clientShape(client_size),
    title_text(title, GameFonts::defaultFont, 12),
    cursor(game_input.cursor)
{
  backShape.setFillColor(gc::DarkGray);
  clientShape.setFillColor(gc::LightGray);

  updatePosition();
}

void GameDialog::update()
{
  if (!game_input.mouseOnScreen) {
    if (grabbed && !game_input.button) {
      backShape.setFillColor(backColor);
      grabbed = false;
    }
    return;
  }
  if (grabbed) {
    if (!game_input.button) {
      backShape.setFillColor(backColor);
      grabbed = false;
      return;
    }
    position.x = cursor.x - grab_origin.x;
    position.y = cursor.y - grab_origin.y;
    clampPosition();
    updatePosition();
    return;
  }

  if (!game_input.button) {
    return;
  }

  const bool is_hover = cursor.x > position.x
                        && cursor.x < position.x + size.x
                        && cursor.y > position.y
                        && cursor.y < position.y + frame_height;
  if (is_hover) {
    backShape.setFillColor(gc::Blue);
    grab_origin.x = cursor.x - position.x;
    grab_origin.y = cursor.y - position.y;
    grabbed = true;
  }
}

void GameDialog::draw(sf::RenderWindow& win)
{
  win.draw(backShape);
  win.draw(clientShape);
  win.draw(title_text);
}

const sf::Vector2f min(0, 0);

void GameDialog::clampPosition()
{
  if (position.x < 0) {
    position.x = 0;
  } else if (position.x + size.x > 900) {
    position.x = 900 - size.x;
  }
  if (position.y < 0) {
    position.y = 0;
  } else if (position.y + size.y > 600) {
    position.y = 600 - size.y;
  }
}

void GameDialog::updatePosition()
{
  client_position.x = position.x + border_width;
  client_position.y = position.y + frame_height;
  backShape.setPosition(position);
  clientShape.setPosition(client_position);

  title_text.setPosition(position.x + 9, position.y + 4);
}