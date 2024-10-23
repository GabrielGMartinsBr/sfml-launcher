
#include "aeon/window/AeonButtonElement.h"

#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>

#include "aeon/enums/AeonElementState.h"
#include "aeon/window/AeonStyleSheet.h"
#include "engnine/base/Fonts.h"

namespace ae {

// Constructor

AeonButtonElement::AeonButtonElement() :
    textFont(nullptr),
    fontSize(16)
{
  setStyle(buttonDefaultStyle);
  applyStyle();
  applyBounds();
}

void AeonButtonElement::drawShapesTo(RenderTarget& target)
{
  refreshValues();
  shape.drawTo(target);
  target.draw(text, sf::BlendAlpha);
}

/*
  ⇩⇩⇩ Window Elements Methods ⇩⇩⇩
*/

const sf::String& AeonButtonElement::setText(const sf::String& value)
{
  textString = value;
  text.setString(textString);
  return textString;
}

void AeonButtonElement::flush()
{
  refreshValues();
}

/*
  ⇩⇩⇩ Private ⇩⇩⇩
*/

void AeonButtonElement::refreshValues()
{
  if (dirtyState) {
    applyState();
    dirtyState = false;
  }
  if (dirtyStyle) {
    applyStyle();
    dirtyStyle = false;
  }
  if (sizeUndefined) {
    adaptToTextSize();
  }
  if (dirtyBounds) {
    applyBounds();
    dirtyBounds = false;
  }
}

void AeonButtonElement::applyBounds()
{
  shape.position(bounds.position());
  shape.size(bounds.size());
  alignText();
}

void AeonButtonElement::applyState()
{
  applyStyle();
  applyStateStyle(AeonElementState::FOCUS);
  applyStateStyle(AeonElementState::HOVER);
  applyStateStyle(AeonElementState::CLICKED);
}

void AeonButtonElement::applyStyle()
{
  applyStyle(defaultStyle);
}

void AeonButtonElement::applyStyle(const AeonStyleSheet& style)
{
  if (style.radius.has_value()) shape.radius(style.radius.value());
  if (style.borderSize.has_value()) shape.borderSize(style.borderSize.value());
  if (style.borderColor.has_value()) shape.borderColor(style.borderColor.value());
  if (style.fillColor.has_value()) shape.fillColor(style.fillColor.value());
  if (style.textColor.has_value()) {
    text.setFillColor(style.textColor.value().getSfColor());
    text.setOutlineThickness(0);
  }
  if (style.fontSize.has_value()) {
    fontSize = style.fontSize.value();
    text.setCharacterSize(fontSize);
  }
  if (style.fontName.has_value()) {
    textFont = Eng::Fonts::Instance().getFont(style.fontName.value());
    if (textFont) {
      text.setFont(*textFont);
    }
  }
}

void AeonButtonElement::applyStateStyle(AeonElementState state)
{
  if (!hasState(state)) {
    return;
  }
  applyStyle(getStateStyle(state));
}

void AeonButtonElement::alignText()
{
  if (!textFont) return;

  text.setCharacterSize(fontSize);

  float leading = textFont->getLineSpacing(fontSize);
  const sf::FloatRect& globalBounds = text.getGlobalBounds();
  const sf::FloatRect& localBounds = text.getLocalBounds();
  float border = defaultStyle.borderSize.value_or(0);
  float totalBorder = border * 2;
  float height = bounds.height() - totalBorder;

  Vector2f textOrigin(
    -(bounds.width() - globalBounds.width - totalBorder) / 2,
    -(height - leading) / 2 
  );

  text.setOrigin(textOrigin);
  text.setPosition(bounds.position());
}

void AeonButtonElement::adaptToTextSize()
{
  if (!textFont || textString.isEmpty()) return;

  const sf::FloatRect& globalBounds = text.getGlobalBounds();
  float px = 0;
  float py = 0;
  float border = 0;
  if (defaultStyle.padding.has_value()) {
    px = defaultStyle.padding.value().x;
    py = defaultStyle.padding.value().y;
  }
  if (defaultStyle.borderSize.has_value()) {
    border = defaultStyle.borderSize.value();
  }
  bounds.width(globalBounds.width + (px + border) * 2);
  bounds.height(globalBounds.height + (py + border) * 2);
}

}  // namespace ae