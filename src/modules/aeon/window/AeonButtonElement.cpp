
#include "aeon/window/AeonButtonElement.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>

namespace ae {

// Constructor

AeonButtonElement::AeonButtonElement(
  const ElementBounds& bounds,
  const AeonStyleSheet& style
) :
    AeonElement(bounds, style)
{
  applyBounds();
  applyStyle();
}

void AeonButtonElement::drawTo(RenderTarget& target)
{
  refreshValues();
  shape.drawTo(target);
}

/*
  ⇩⇩⇩ Window Elements Methods ⇩⇩⇩
*/

const sf::String& AeonButtonElement::setText(const sf::String& value)
{
  text = value;
  return text;
}

/*
  ⇩⇩⇩ Private ⇩⇩⇩
*/

void AeonButtonElement::refreshValues()
{
  if (dirtyBounds) {
    applyBounds();
    dirtyBounds = false;
  }
  if (dirtyStyle) {
    applyStyle();
    dirtyStyle = false;
  }
}

void AeonButtonElement::applyBounds()
{
  shape.position(bounds.position());
  shape.size(bounds.size());
}

void AeonButtonElement::applyStyle()
{
  shape.borderSize(defaultStyle.borderSize);
  shape.borderColor(defaultStyle.borderColor.getSfColor());
  shape.fillColor(defaultStyle.bgColor.getSfColor());
}

}  // namespace ae